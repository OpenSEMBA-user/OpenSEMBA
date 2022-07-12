#include "Parser.h"

#include "parsers/stl/Parser.h"

#include "math/function/Gaussian.h"
#include "math/function/BandLimited.h"
#include "model/Model.h"
#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Tetrahedron4.h"
#include "geometry/element/Hexahedron8.h"
#include "physicalModel/Bound.h"
#include "physicalModel/multiport/Dispersive.h"
#include "physicalModel/multiport/Predefined.h"
#include "physicalModel/multiport/RLC.h"
#include "physicalModel/Predefined.h"
#include "physicalModel/volume/AnisotropicCrystal.h"
#include "physicalModel/volume/AnisotropicFerrite.h"
#include "physicalModel/volume/Classic.h"
#include "physicalModel/volume/PML.h"
#include "physicalModel/wire/Wire.h"
#include "physicalModel/Gap.h"
#include "physicalModel/PriorityMaterial.h"
#include "source/port/WaveguideRectangular.h"
#include "source/port/TEMCoaxial.h"
#include "outputRequest/BulkCurrent.h"
#include "outputRequest/FarField.h"
#include "outputRequest/OnPoint.h"
#include "outputRequest/OnLine.h"
#include "outputRequest/OnSurface.h"
#include "outputRequest/OnLayer.h"
#include "string"

using namespace SEMBA::Geometry;

namespace SEMBA {
namespace Parsers {
namespace JSON {

double getProgressionStepByTotalNumber(const json& j, const std::string& jsonKey) {
    if (j.find("total" + jsonKey) != j.end()) {
        int total = j.at("total" + jsonKey).get<int>();

        // TODO: Yet to be clarify differences between UI / JSON
        auto step = j.at("final" + jsonKey) - j.at("initial" + jsonKey).get<double>();
        if (total > 1) {
            step = step / (total - 1);
        }

        return step;

    }

    if (j.find("step" + jsonKey) != j.end()) {
        return j.at("step" + jsonKey).get<double>();
    }

    // TODO: Check better approach
    std::string toLower = jsonKey;
    for (auto& c : toLower) {
        c = tolower(c);
    }

    return j.at(toLower + "Step").get<double>();
}

std::vector<const Geometry::CoordR3*> addAngGetCoordView(
    CoordR3Group& cG,
    const std::vector<Math::CVecR3>& positions,
    const size_t& numberOfCoordinates
) {
    std::vector<const Geometry::CoordR3*> coords;

    for (std::size_t i = 0; i < numberOfCoordinates; i++) {
        coords.push_back(
            cG.addAndAssignId(
                std::make_unique<Geometry::CoordR3>(Geometry::CoordId(), positions[i])
            )->get()
        );
    }

    return coords;
}

ProblemDescription Parser::readExtended() const {
	std::ifstream stream(this->filename);
	if (!stream.is_open()) {
		throw std::logic_error("Can not open file: " + this->filename);
	}

	json j;
	try {
		stream >> j;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	checkExtendedVersionCompatibility(
        j.at("_version").get<std::string>()
    );

	ProblemDescription res;
	res.analysis = readSolverOptions(j, "analysis");
	res.grids = this->readGrids(j);

	auto materialsGroup = this->readExtendedPhysicalModels(j);
	auto mesh = this->readUnstructuredMesh(materialsGroup, j.at("model"));

	res.sources = this->readSources(*mesh, j);
	res.outputRequests = readOutputRequests(*mesh, j, "probes");

	this->readBoundary(j, *mesh, materialsGroup, res.grids);

	res.model = Model::Model(*mesh, materialsGroup);

	return res;
}

Parser::Parser(const std::string& fn) : SEMBA::Parsers::Parser(fn) {}

Data Parser::read() const {
    
    std::ifstream stream(this->filename);
    if (!stream.is_open()) {
        throw std::logic_error("Can not open file: " + this->filename);
    }
    
    json j;
    try {
        stream >> j;
    }
    catch( const std::exception & ex ) {
        std::cerr << ex.what() << std::endl;
    }

    std::string version = j.at("_version").get<std::string>();
    if (!checkVersionCompatibility(version)) {
        throw std::logic_error("File version " + version + " is not supported.");
    }


    Data res;
    res.filename = this->filename;
    res.solver = readSolverOptions(j);
    res.grids = this->readGrids(j);
    res.physicalModels = readPhysicalModels(j);
    res.mesh = readUnstructuredMesh(res.physicalModels, j);

    if (res.mesh != nullptr) {
        Mesh::Unstructured* mesh = res.mesh->castTo<Mesh::Unstructured>();
		readConnectorOnPoint(res.physicalModels, *mesh, j);
        res.sources = readSources(*mesh, j);
        res.outputRequests = readOutputRequests(*mesh, j);
    } else {
        res.sources = Source::Group<>();
        res.outputRequests = OutputRequest::Group<>();
    }

    postReadOperations(res);

    return res;
}

void Parser::readBoundary(
    const json& j, 
    Geometry::Mesh::Unstructured& mesh, 
    PMGroup& physicalModelGroup, 
    const Geometry::Grid3& grid
) const {
    json lower = j.at("boundary").at("lower");
    json upper = j.at("boundary").at("upper");

    if (lower.size() != 3 || upper.size() != 3) {
        throw std::logic_error("Unexpected amount of materials for boundary layers specified. Three layers are expected");
    }

    const auto& box = grid.getFullDomainBoundingBox();

    for (const auto& bound : { Math::Constants::CartesianBound::L, Math::Constants::CartesianBound::U }) {
        const auto& boundaryResource = bound == Math::Constants::CartesianBound::L ? lower : upper;

        for (const auto& axis : { Math::Constants::CartesianAxis::x,Math::Constants::CartesianAxis::y,Math::Constants::CartesianAxis::z }) {
            PhysicalModel::Id id(0);

            for (const auto& boundI : physicalModelGroup.getOf<PhysicalModel::Bound>()) {
                if (boundI->getType() == this->strToBoundType(boundaryResource[axis].get<std::string>())) {
                    id = boundI->getId();
                    break;
                }
            }

            if (id == PhysicalModel::Id(0)) {
                auto it = physicalModelGroup.addAndAssignId(
                    std::make_unique<PhysicalModel::Bound>(PhysicalModel::Id(), this->strToBoundType(boundaryResource[axis].get<std::string>()))
                );

                id = (*it)->getId();
            }

            const auto& boxBoundary = box.getPosOfBound(axis, bound);
            std::array<const CoordR3*, 4> boundCoordinatesArray{};
            for (std::size_t boxBoundaryIndex = 0; boxBoundaryIndex < boxBoundary.size(); ++boxBoundaryIndex) {
                const auto& coordIt = mesh.coords().addAndAssignId(
                    std::make_unique<CoordR3>(boxBoundary[boxBoundaryIndex])
                );

                boundCoordinatesArray[boxBoundaryIndex] = coordIt->get();
            }

            mesh.elems().addAndAssignId(
                std::make_unique<QuaR4>(
                    ElemId(),
                    boundCoordinatesArray,
                    nullptr,
                    physicalModelGroup.getId(id)
                )
            );
        }
    }
}

PhysicalModel::Bound::Type Parser::strToBoundType(const std::string& boundType) const {
    if (boundType == "PEC") {
        return PhysicalModel::Bound::Type::pec;
    }

    if (boundType == "PMC") {
        return PhysicalModel::Bound::Type::pmc;
    }
    
    if (boundType == "PML") {
        return PhysicalModel::Bound::Type::pml;
    }
    
    if (boundType == "Periodic") {
        return PhysicalModel::Bound::Type::periodic;
    }
    
    if (boundType == "MUR1") {
        return PhysicalModel::Bound::Type::mur1;
    }
    
    if (boundType == "MUR2") {
        return PhysicalModel::Bound::Type::mur2;
    }
    
    throw std::logic_error("Unrecognized value in Bound ctor.");
}

Parser::json Parser::readSolverOptions(const json& j, const std::string& key) const
{
    return j.find(key) == j.end() ? json(): j.at(key).get<json>();
}

std::unique_ptr<Mesh::Unstructured> Parser::readUnstructuredMesh(const PMGroup& physicalModels, const json& j) const
{
    Layer::Group<> layers = readLayers(j);
	CoordR3Group coords = readCoordinates(j);
	return std::make_unique<Mesh::Unstructured>(
		coords,
		readElements(physicalModels, layers, coords, j),
		layers
	);
}

Source::Group<> Parser::readSources(Mesh::Unstructured& mesh, const json& j) const 
{
    auto sources = j.find("sources");
    
    Source::Group<> res = Source::Group<>();
    if (sources == j.end()) {
        return res;
    }

    for (auto const& it: sources->get<json>()) {
        std::string sourceType = it.at("sourceType").get<std::string>();
        if (sourceType.compare("planewave") == 0) {
            res.addAndAssignId(readPlanewave(mesh, it));
            continue;
        } 
        
        if (sourceType.compare("generator") == 0) {
            res.addAndAssignId(readGenerator(mesh, it));
            continue;
        } 
        
        if (sourceType.compare("sourceOnLine") == 0) {
            res.addAndAssignId(readSourceOnLine(mesh, it));
            continue;
        } 
        
        if (sourceType.compare("waveguidePort") == 0) {
            res.addAndAssignId(readPortWaveguide(mesh, it));
            continue;
        } 
        
        if (sourceType.compare("temPort") == 0) {
            res.addAndAssignId(readPortTEM(mesh, it));
            continue;
        } 

        throw std::logic_error("Unrecognized source type: " + sourceType);
        
    }
    return res;
}

PMGroup Parser::readPhysicalModels(const json& j) const 
{
    PMGroup res;
    for (auto const& mat: j.at("materials")) {
        res.addAndAssignId(readPhysicalModel( mat ));
    }
    return res;
}

PMGroup Parser::readExtendedPhysicalModels(const json& j) const
{
    PMGroup res;
    for (auto const& mat: j.at(json::json_pointer("/model/materials"))) {
        res.add(readPhysicalModel( mat ));
    }
    return res;
}

std::unique_ptr<PhysicalModel::PhysicalModel> Parser::readPhysicalModel(const json& j) const 
{
    typedef PhysicalModel::PhysicalModel PM;
    
    PM::Type type = strToMaterialType( j.at("materialType").get<std::string>() );
	MatId id(0);
	if (j.find("materialId") != j.end()) {
		id = MatId(j.at("materialId").get<int>());
	}

	std::string name;
	if (j.find("name") != j.end()) {
		name = j.at("name").get<std::string>();
	}

    switch (type) {
    case PM::Type::PEC:
        return std::make_unique<PhysicalModel::PEC>(id, name);
    case PM::Type::PMC:
        return std::make_unique<PhysicalModel::PMC>(id, name);
    case PM::Type::SMA:
        return std::make_unique<PhysicalModel::SMA>(id, name);
    case PM::Type::PML:
        return std::make_unique<PhysicalModel::Volume::PML>(id, name, 
			strToLocalAxes(j.at("localAxes").get<std::string>()));

    case PM::Type::classic:
        return std::make_unique<PhysicalModel::Volume::Classic>(
                id, 
                name,
                j.at("permittivity").get<double>(),
                j.at("permeability").get<double>(),
                j.at("electricConductivity").get<double>(),
                j.at("magneticConductivity").get<double>()
            );

    case PM::Type::elecDispersive:
        return std::make_unique <PhysicalModel::Volume::Dispersive>(
                id, 
                name,
                j.at("filename").get<std::string>()
            );

    case PM::Type::wire:
    {
        std::string wireType = j.at("wireType").get<std::string>();
        if (wireType.compare("Dispersive") == 0) {
            return std::make_unique < PhysicalModel::Wire::Wire>(id, name,
                    j.at("radius").get<double>(),
                    j.at("filename").get<std::string>());
        } else if(wireType.compare("SeriesParallel") == 0) {
            return std::make_unique < PhysicalModel::Wire::Wire>(id, name,
                    j.at("radius").get<double>(),
                    j.at("resistance").get<double>(),
                    j.at("inductance").get<double>(),
                    j.at("capacitance").get<double>(),
                    j.at("parallelResistance").get<double>(),
                    j.at("parallelInductance").get<double>(),
                    j.at("parallelCapacitance").get<double>());
        } else if(wireType.compare("Standard") == 0) {
            return std::make_unique < PhysicalModel::Wire::Wire>(id, name,
                    j.at("radius").get<double>(),
                    j.at("resistance").get<double>(),
                    j.at("inductance").get<double>());
        } else {
            throw std::logic_error("Unrecognized wire type" + wireType);
        }
    }

    case PM::Type::anisotropic:
    {
        std::string str = j.at("anisotropicModel").get<std::string>();
        if (str.compare("Crystal")==0) {
            return std::make_unique < PhysicalModel::Volume::AnisotropicCrystal>(id, name,
                    strToLocalAxes(j.at("localAxes").get<std::string>()),
                    strToCVecR3(
                            j.at("relativePermittiviy").get<std::string>()),
                    j.at("crystalRelativePermeability").get<double>());
        } else if (str.compare("Ferrite")==0) {
            return std::make_unique < PhysicalModel::Volume::AnisotropicFerrite>(id, name,
                    strToLocalAxes(j.at("localAxes").get<std::string>()),
                    j.at("kappa").get<double>(),
                    j.at("ferriteRelativePermeability").get<double>(),
                    j.at("ferriteRelativePermittivity").get<double>());
        } else {
            throw std::logic_error("Unrecognized Anisotropic Model: " + str);
        }
    }

    case PM::Type::isotropicsibc:
    {
        std::string sibcType = j.at("surfaceType").get<std::string>();
        if (sibcType.compare("File")==0) {
            return std::make_unique < PhysicalModel::Surface::SIBCFile>(id, name,
                    j.at("filename").get<std::string>() );
        } else if (sibcType.compare("Layers")==0) {
            return readMultilayerSurface(j);
        } else {
            throw std::logic_error("Unrecognized SIBC type: " + sibcType);
        }
    }

    case PM::Type::gap:
        return std::make_unique < PhysicalModel::Gap>(id, name, j.at("width").get<double>());

    case PM::Type::multiport:
    {
        using namespace PhysicalModel::Multiport;
        auto mpType = strToMultiportType(j.at("connectorType").get<std::string>());
        switch (mpType) {
        case Multiport::Type::shortCircuit:
            return std::make_unique<Predefined>(id, name, mpType);
        case Multiport::Type::openCircuit:
			return  std::make_unique<Predefined>(id, name, mpType);
        case Multiport::Type::dispersive:
            return  std::make_unique<Dispersive>(id, name, j.at("filename").get<std::string>());
        default:
            return  std::make_unique<RLC>(id, name, mpType,
                    j.at("resistance").get<double>(),
                    j.at("inductance").get<double>(),
                    j.at("capacitance").get<double>());
        }
    }

    case PM::Type::priorityMaterial:
    {
        return std::make_unique<PhysicalModel::PriorityMaterial>(
            id,
            name,
            j.at("priority").get<int>()
        );
    }

    default:
        throw std::logic_error("Material type not recognized for: " + name);
    }
}

OutputRequest::Group<> Parser::readOutputRequests(Mesh::Unstructured& mesh, const json& j, const std::string& key) const 
{
    OutputRequest::Group<> res;
    
    auto outs = j.find(key);
    if (outs == j.end()) {
        return res;
    }

    for (auto const& out: outs->get<json>()) {
        res.addAndAssignId(readOutputRequest(mesh, out));
    }

    return res;
}

void Parser::readConnectorOnPoint(PMGroup& pMG, Mesh::Unstructured& mesh, const json& j) const 
{
    auto conns = j.find("connectorOnPoint");
    if (conns == j.end()) {
        return;
    }

	for (auto const& it: conns->get<json>()) {
		PhysicalModel::PhysicalModel* mat = pMG.addAndAssignId(readPhysicalModel(it))->get();
		const CoordR3* coord[1] = { mesh.coords().getId(CoordId(it.at("coordIds").get<int>())) };

		mesh.elems().addAndAssignId(
            std::make_unique<NodR>(ElemId(0), coord, nullptr, pMG.getId(mat->getId()))
        );
	}
}

const ElemR* Parser::boxToElemGroup(Mesh::Unstructured& mesh, const std::string& line)
{
    BoxR3 box = strToBox(line);
    std::unique_ptr<ElemR> elem;

    auto posVec = box.getPos();

    if (box.isVolume()) {    
        elem = std::make_unique<HexR8>(ElemId(), addAngGetCoordView(mesh.coords(), posVec, Geometry::HexR8::sizeOfCoordinates).data());
    } else if (box.isSurface()) {
        elem = std::make_unique<QuaR4>(ElemId(), addAngGetCoordView(mesh.coords(), posVec, Geometry::QuaR4::sizeOfCoordinates).data());
    } else if (box.isLine()) {
		elem = std::make_unique<LinR2>(ElemId(), addAngGetCoordView(mesh.coords(), posVec, Geometry::LinR2::sizeOfCoordinates).data());
	} else if (box.isPoint()) {
        elem = std::make_unique<NodR>(ElemId(), addAngGetCoordView(mesh.coords(), posVec, 1).data());
	} else {
        throw std::logic_error("Box to Elem Group only works for volumes and surfaces");
    }

    return mesh.elems().addAndAssignId(std::move(elem))->get();
}

std::unique_ptr<OutputRequest::OutputRequest> Parser::readOutputRequest(Mesh::Unstructured& mesh, const json& j) {

    std::string name = j.at("name").get<std::string>();
    OutputRequest::OutputRequest::Type type = strToOutputType(j.at("type").get<std::string>());
    std::string gidOutputType = j.at("gidOutputType").get<std::string>();
    OutputRequest::Domain domain = readDomain(j.at("domain").get<json>());

	if (type == OutputRequest::OutputRequest::Type::bulkCurrentElectric ||
		type == OutputRequest::OutputRequest::Type::bulkCurrentMagnetic) {
		if (gidOutputType.compare("OutRq_on_layer") == 0) {
			return std::make_unique<OutputRequest::BulkCurrent>(
				OutputRequest::BulkCurrent(
					domain,
					name,
					{ boxToElemGroup(mesh, j.at("box").get<std::string>())->castTo<NodR>() },
					strToCartesianAxis(j.at("direction").get<std::string>()),
					j.at("skip").get<int>()
				)
			);
        }

        if (gidOutputType.compare("OutRq_on_point") == 0) {
			return std::make_unique<OutputRequest::BulkCurrent>(
				OutputRequest::BulkCurrent(
					domain,
					name,
					readCoordIdAsNodes(mesh, j.at("elemIds").get<json>()),
					strToCartesianAxis(j.at("direction").get<std::string>()),
					j.at("skip").get<int>()
				)
			);
        }

		return std::make_unique<OutputRequest::BulkCurrent>(
			OutputRequest::BulkCurrent(
				domain,
				name,
				readElemIdsAsGroupOf(mesh, j.at("elemIds").get<json>()),
				strToCartesianAxis(j.at("direction").get<std::string>()),
				j.at("skip").get<int>()
			)
		);
    }

    if (gidOutputType.compare("OutRq_on_point") == 0) {        
        ElemView target;
        if (j.contains("positions")) {
            target = readAndCreateCoordIdAsNodes(mesh, j.at("positions").get<json>());
        } else {
            target = readCoordIdAsNodes(mesh, j.at("elemIds").get<json>());
        }
        
		return std::make_unique<OutputRequest::OnPoint>(type, domain, name, target);
    }
    
    if (gidOutputType.compare("OutRq_on_line") == 0) {
		return std::make_unique<OutputRequest::OnLine>(
			OutputRequest::OnLine(
				type,
				domain,
				name,
				readElemIdsAsGroupOf(mesh, j.at("elemIds").get<json>())
			)
		);
    }

    if (gidOutputType.compare("OutRq_on_surface") == 0) {
		return std::make_unique<OutputRequest::OnSurface>(
			OutputRequest::OnSurface(
				type,
				domain,
				name,
				readElemIdsAsGroupOf(mesh, j.at("elemIds").get<json>())
			)
		);
    }

    if (gidOutputType.compare("OutRq_on_layer") == 0) {
		return std::make_unique<OutputRequest::OnLayer>(
			OutputRequest::OnLayer(
				type,
				domain,
				name,
				{ boxToElemGroup(mesh, j.at("box").get<std::string>()) }
			)
		);
    }

    if (gidOutputType.compare("Far_field") == 0) {
		static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
		return std::make_unique<OutputRequest::FarField>(
			OutputRequest::FarField(
				domain,
				name,
				{ boxToElemGroup(mesh, j.at(
                    j.find("layerBox") == j.end() ? "box" : "layerBox"
                ).get<std::string>()) },
				j.at("initialTheta").get<double>() * degToRad,
				j.at("finalTheta").get<double>() * degToRad,
                getProgressionStepByTotalNumber(j, "Theta") * degToRad,
				j.at("initialPhi").get<double>() * degToRad,
				j.at("finalPhi").get<double>() * degToRad,
                getProgressionStepByTotalNumber(j, "Phi") * degToRad
            )
		);
    }

    throw std::logic_error("Unrecognized GiD Output request type: " + gidOutputType);
}

ElemView Parser::readAndCreateCoordIdAsNodes(Geometry::Mesh::Unstructured& mesh, const Parser::json& j) {
	ElemView res;

	for (auto it = j.begin(); it != j.end(); ++it) {
		auto coordinates = (*it).get<std::vector<Math::Real>>();

        const auto coordIt = mesh.coords().addAndAssignId(
            std::make_unique<CoordR3>((Math::CVecR3(coordinates.data())))
        );

        const CoordR3* constCoordPointer[1] = { coordIt->get() };
		auto itCoord = mesh.elems().addAndAssignId(
            std::make_unique<NodR>(ElemId(), constCoordPointer)
        );

		res.push_back(itCoord->get());
	}

	return res;
}

Math::Constants::CartesianAxis Parser::strToCartesianAxis(std::string str) {
    if (str.compare("x") == 0) {
        return Math::Constants::x;
    } else if (str.compare("y") == 0) {
        return Math::Constants::y;
    } else if (str.compare("z") == 0) {
        return Math::Constants::z;
    } else {
        throw std::logic_error("Unrecognized cartesian axis label: " + str);
    }
}

LayerGroup Parser::readLayers(const json& j) const {
    if (j.find("layers") == j.end()) {
        throw std::logic_error("layers object was not found.");
    }

    LayerGroup res;
    for (auto const& it: j.at("layers")) {
        res.add(
            std::make_unique<Layer::Layer>(
                Layer::Id(it.at("id").get<int>()),
                it.at("name").get<std::string>()
            )
        );
    }
    return res;
}

CoordR3Group Parser::readCoordinates(const json& j) const {

    if (j.find("coordinates") == j.end()) {
        throw std::logic_error("Coordinates label was not found.");
    }

    CoordR3Group res;
    const json& c = j.at("coordinates").get<json>();
    for (json::const_iterator it = c.begin(); it != c.end(); ++it) {
        CoordId id;
        Math::CVecR3 pos;
        std::stringstream ss(it->get<std::string>());
        ss >> id >> pos(0) >> pos(1) >> pos(2);
        res.add(std::make_unique<CoordR3>(id, pos));
    }
    return res;
}

ElemRGroup Parser::readElements(
        const PMGroup& mG, LayerGroup& lG, CoordR3Group& cG, const json& j) const
{

    if (j.find("elements") == j.end()) {
        throw std::logic_error("Elements label was not found.");
    }

    ElemRGroup res;
    const json& elems = j.at("elements").get<json>();

	// Check how to iterate over, somehow
	if (elems.find("hexahedra") != elems.end()) {
		res.addAndAssignIds(readElemStrAs<HexR8>(mG, lG, cG, elems.at("hexahedra").get<json>()));
	}
	if (elems.find("tetrahedra") != elems.end()) {
		res.addAndAssignIds(readElemStrAs<Tet4>(mG, lG, cG, elems.at("tetrahedra").get<json>()));
	}
	if (elems.find("quadrilateral") != elems.end()) {
		res.addAndAssignIds(readElemStrAs<QuaR4>(mG, lG, cG, elems.at("quadrilateral").get<json>()));
	}
	if (elems.find("triangle") != elems.end()) {
		res.addAndAssignIds(readElemStrAs<Tri3>(mG, lG, cG, elems.at("triangle").get<json>()));
	}
	if (elems.find("line") != elems.end()) {
		res.addAndAssignIds(readElemStrAs<LinR2>(mG, lG, cG, elems.at("line").get<json>()));
	}

	if (elems.find("fromFile") != elems.end()) {
		res.addAndAssignIds(readElementsFromFile(mG, lG, cG, elems.at("fromFile").get<json>()));
	}

    return res;
}

ElemRGroup Parser::readElementsFromSTLFile(
    const PMGroup& mG, LayerGroup& lG, CoordR3Group& cG, const json& f) const
{
    std::string fn = this->filename.getFolder() + f.at("file").get<std::string>();
    Mesh::Unstructured m = Parsers::STL::Parser(fn).readAsUnstructuredMesh();

    auto lay = lG.getId( LayerId(f.at("layerId").get<std::size_t>())  );
    auto mat = mG.getId( MatId(f.at("materialId").get<std::size_t>()) );
    
	ElemRGroup res;
	for (const auto& elem : m.elems()) {
		std::vector<const CoordR3*> vs;
		for (std::size_t i = 0; i < elem->numberOfVertices(); i++) {
			if (!cG.existId(elem->getV(i)->getId())) {
				cG.addAndAssignId(
					std::make_unique<CoordR3>(*elem->getV(i))
				);
			}

			vs.push_back(cG.getId(elem->getV(i)->getId()));
		}

		res.addAndAssignId(std::make_unique<Tri3>(ElemId(0), vs.data(), lay, mat));
	}

    return res;
}

ElemRGroup Parser::readElementsFromFile(
    const PMGroup& mG,
    LayerGroup& lG,
    CoordR3Group& cG,
    const json& eFile) const
{
    ElemRGroup res;
    for (auto const& f : eFile) {
        if (f.find("format") == f.end()) {
            throw std::runtime_error("Format label not found when reading elements from file.");
        }

        std::string format = f.at("format").get<std::string>();
        if (format == "STL") {
            res.addAndAssignIds(readElementsFromSTLFile(mG, lG, cG, f));

            continue;
		}
		
        if (format == "ignore") {
			// Ignoring file
            continue;
		}

		throw std::runtime_error("Unsupported file format when reading elements from file.");
	}
    return res;
}

std::unique_ptr<PhysicalModel::Surface::Multilayer> 
Parser::readMultilayerSurface(const json& mat) const 
{
    MatId id = MatId(mat.at("materialId").get<int>());
    std::string name = mat.at("name").get<std::string>();

    std::vector<PhysicalModel::Surface::Multilayer::Layer> layers;
    for (json::const_iterator it = mat.at("layers").begin(); it != mat.at("layers").end(); ++it) {
        layers.push_back(PhysicalModel::Surface::Multilayer::Layer(
            it->at("thickness").get<double>(),
            it->at("permittivity").get<double>(),
            it->at("permeability").get<double>(),
            it->at("elecCond").get<double>() )
        );
    }

    if (mat.at("useSembaVectorFitting").get<bool>()) {
        PhysicalModel::Surface::Multilayer::FittingOptions opts(
                std::make_pair(mat.at("freqMin").get<double>(),
                        mat.at("freqMax").get<double>()),
                        mat.at("numberOfPoles").get<int>());
        std::vector<PhysicalModel::Surface::Multilayer::FittingOptions> optsVec = { opts };
        return std::make_unique<PhysicalModel::Surface::Multilayer>(id, name, layers, optsVec);
    } else {
        return std::make_unique < PhysicalModel::Surface::Multilayer>(id, name, layers);
    }
}

Grid3 Parser::readGrids(const json& j) const {
    if (j.find("grids") == j.end()) {
        throw std::logic_error("Grids object not found.");
    }

    json g = j.at("grids").front();
    std::string gridType = g.at("gridType").get<std::string>();
    if (gridType.compare("gridCondition") == 0) {
        // Initializes basic grid.
        Grid3 res;
        if (g.at("type").get<std::string>().compare("Number_of_cells") == 0) {
            res = Grid3(
                    strToBox(g.at("layerBox").get<std::string>()),
                    strToCVecI3(g.at("numberOfCells").get<std::string>()));
        } else {
            BoxR3 box = strToBox(g.at("layerBox").get<std::string>());
            Math::CVecR3 stepSize = strToCVecR3(g.at("stepSize").get<std::string>());
            if (g.at("fitSizeToBox").get<bool>()) {
                for (std::size_t i = 0; i < 3; i++) {
					std::size_t n = std::round(box.getLength()[i] / stepSize[i]);
					if (n == 0) {
						n = 1;
					}
                    stepSize[i] = box.getLength()[i] / n;
                }
            }
            res = Grid3(box, stepSize);
        }

        // Applies boundary padding operations.
        if (g.find("lowerPaddingMeshSize") != g.end()) {
            std::pair<Math::CVecR3, Math::CVecR3> boundaryMeshSize(
                    strToCVecR3(g.at("lowerPaddingMeshSize").get<std::string>()),
                    strToCVecR3(g.at("upperPaddingMeshSize").get<std::string>())
            );
            std::pair<Math::CVecR3, Math::CVecR3> boundaryPadding(
                    strToCVecR3(g.at("lowerPadding").get<std::string>()),
                    strToCVecR3(g.at("upperPadding").get<std::string>())
            );
            if (g.at("boundaryPaddingType").get<std::string>().compare(
                    "by_number_of_cells") == 0) {
                boundaryPadding.first  *= boundaryMeshSize.first;
                boundaryPadding.second *= boundaryMeshSize.second;
            }
            res.enlarge(boundaryPadding, boundaryMeshSize);
        }

		if (res.hasZeroSize()) {
			throw std::logic_error("Grid has zero size.");
		}
        return res;

    } else if (gridType.compare("nativeGiD") == 0) {
        Math::CVecR3 corner = strToCVecR3(g.at("corner").get<std::string>());
        Math::CVecR3 boxSize = strToCVecR3(g.at("boxSize").get<std::string>());
        Math::CVecI3 nGridPoints = strToCVecI3(g.at("nGridPoints").get<std::string>());
        std::vector<Math::Real> pos[3];
        pos[0] = g.at("xCoordinates").get<std::vector<double>>();
        pos[1] = g.at("yCoordinates").get<std::vector<double>>();
        pos[2] = g.at("zCoordinates").get<std::vector<double>>();
        if (!pos[0].empty()) {
            return Grid3(pos);
        } else {
            std::pair<Math::CVecR3, Math::CVecR3> box =
            { corner, corner + boxSize };
            return Grid3(box, nGridPoints);
        }
    } else if (gridType.compare("positionsFromFile") == 0) {
        std::string folder = this->filename.getFolder();
        std::string projectName = this->filename.getProjectName();
        return buildGridFromFile(folder + projectName + ".grid.json");
    } else {
        throw std::logic_error("Unrecognized grid type: " + gridType);
    }
}

Grid3 Parser::buildGridFromFile(const FileSystem::Project& jsonFile) const
{
    if (!jsonFile.canOpen()) {
        throw std::logic_error("ERROR @ Parser::JSON: Unable to open grid file");
    }
    std::ifstream fileStream(jsonFile);
    json jsonObj;
    try {
        fileStream >> jsonObj;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    
    auto gridIt = jsonObj.find("grid");
    if (gridIt != jsonObj.end()) {
        std::vector<Math::Real> pos[3] = {
            gridIt->at("xs").get<std::vector<double>>(),
            gridIt->at("ys").get<std::vector<double>>(),
            gridIt->at("zs").get<std::vector<double>>()
        };
        if (!pos[0].empty() && !pos[1].empty() && !pos[2].empty()) {
            return Grid3(pos);
        }
        else {
            throw std::logic_error("Grid file had empty positions in at least one direction");
        }
    }
    else {
        throw std::logic_error("Grid file did not contain a grid label");
    }
}

std::unique_ptr<Source::PlaneWave> Parser::readPlanewave(Mesh::Unstructured& mesh, const json& j) {
    
    auto magnitude = readMagnitude(j.at("magnitude").get<json>());
    Geometry::ElemView elems = { boxToElemGroup(mesh, j.at("layerBox").get<std::string>()) };
	if (elems.size() == 0) {
		throw std::logic_error("Plane wave layer must define a volume");
	}
    
    std::string definitionMode = j.at("definitionMode").get<std::string>();
    if (definitionMode.compare("by_vectors")==0) {
		return std::make_unique<Source::PlaneWave>(
			Source::PlaneWave(
				magnitude,
				elems,
				strToCVecR3(j.at("directionVector").get<std::string>()),
				strToCVecR3(j.at("polarizationVector").get<std::string>())
			)
		);
    } else if (definitionMode.compare("by_angles")==0) {
        static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
        std::pair<Math::Real,Math::Real> dirAngles, polAngles;
        dirAngles.first  = j.at("directionTheta").get<double>()    * degToRad;
        dirAngles.second = j.at("directionPhi").get<double>()      * degToRad;
        polAngles.first  = j.at("polarizationAlpha").get<double>() * degToRad;
        polAngles.second = j.at("polarizationBeta").get<double>()  * degToRad;
		return std::make_unique<Source::PlaneWave>(
			Source::PlaneWave(
				magnitude,
				elems,
				dirAngles,
				polAngles
			)
		);

    } else if (definitionMode.compare("randomized_multisource")==0) {
		return std::make_unique<Source::PlaneWave>(
			Source::PlaneWave(
				magnitude,
				elems,
				j.at("numberOfRandomPlanewaves").get<int>(),
				j.at("relativeVariationOfRandomDelay").get<double>()
			)
        );
    } else {
        throw std::logic_error("Unrecognized label: " + definitionMode);
    }
}

std::unique_ptr<Source::Port::Waveguide> Parser::readPortWaveguide(
    Mesh::Unstructured& mesh, 
    const json& j
) {
	std::string shape = j.at("shape").get<std::string>();
	if (shape.compare("Rectangular") == 0) {
		return std::make_unique<Source::Port::WaveguideRectangular>(
			Source::Port::WaveguideRectangular(
				readMagnitude(j.at("magnitude").get<json>()),
				readElemIdsAsGroupOf(
					mesh,
					j.at("elemIds").get<json>()
				),
				strToWaveguideMode(j.at("excitationMode").get<std::string>()),
				{ j.at("firstMode").get<int>(), j.at("secondMode").get<int>() }
			)
		);
	}
	else {
		throw std::logic_error("Unrecognized waveguide port shape: " + shape);
	}
}

std::unique_ptr<Source::Port::TEM> Parser::readPortTEM(
	Mesh::Unstructured& mesh,
	const json& j
) {
	return std::make_unique<Source::Port::TEMCoaxial>(
		Source::Port::TEMCoaxial(
			readMagnitude(j.at("magnitude").get<json>()),
			readElemIdsAsGroupOf(mesh, j.at("elemIds").get<json>()),
			strToTEMMode(j.at("excitationMode").get<std::string>()),
			strToCVecR3(j.at("origin").get<std::string>()),
			j.at("innerRadius").get<double>(),
			j.at("outerRadius").get<double>()
		)
	);
}

std::unique_ptr<Source::Generator> Parser::readGenerator(
    Mesh::Unstructured& mesh, 
    const json& j
) {
	return std::make_unique<Source::Generator>(
		Source::Generator(
			readMagnitude(j.at("magnitude").get<json>()),
			readCoordIdAsNodes(mesh, j.at("coordIds").get<json>()),
			strToGeneratorType(j.at("type").get<std::string>()),
			Source::Generator::soft
        )
	);
            //strToGeneratorHardness(j.at("hardness").get<std::string>()) );
}

std::unique_ptr<Source::OnLine> Parser::readSourceOnLine(
	Mesh::Unstructured& mesh, 
    const json& j) {
	return std::make_unique<Source::OnLine>(
		Source::OnLine(
			readMagnitude(j.at("magnitude").get<json>()),
			readElemIdsAsGroupOf(
				mesh,
				j.at("elemIds").get<json>()
			),
			strToNodalType(j.at("type").get<std::string>()),
			strToNodalHardness(j.at("hardness").get<std::string>())
        )
	);
}

OutputRequest::OutputRequest::Type Parser::strToOutputType(std::string str) {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return OutputRequest::OutputRequest::Type::electric;
    }
    else if (str.compare("magneticField") == 0) {
        return OutputRequest::OutputRequest::Type::magnetic;
    }
    else if (str.compare("electricFieldNormals") == 0) {
        return OutputRequest::OutputRequest::Type::electricFieldNormals;
    }
    else if (str.compare("magneticFieldNormals") == 0) {
        return OutputRequest::OutputRequest::Type::magneticFieldNormals;
    }
    else if (str.compare("current") == 0) {
        return OutputRequest::OutputRequest::Type::current;;
    }
    else if (str.compare("bulkCurrentElectric") == 0) {
        return OutputRequest::OutputRequest::Type::bulkCurrentElectric;
    }
    else if (str.compare("bulkCurrentMagnetic") == 0) {
        return OutputRequest::OutputRequest::Type::bulkCurrentMagnetic;
    }
    else if (str.compare("surfaceCurrentDensity") == 0) {
        return OutputRequest::OutputRequest::Type::surfaceCurrentDensity;
    }
    else if (str.compare("farField") == 0) {
        return OutputRequest::OutputRequest::Type::electric;
    } else {
        throw std::logic_error("Unrecognized output type: " + str);
    }
}

Source::Generator::Type Parser::strToGeneratorType(std::string str) {
    str = trim(str);
    if (str.compare("voltage")==0) {
        return Source::Generator::voltage;
    }
    
    if (str.compare("current")==0) {
        return Source::Generator::current;
    }

    throw std::logic_error("Unrecognized generator type: " + str);
}

Source::Generator::Hardness Parser::strToGeneratorHardness(std::string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Source::Generator::soft;
    } else if (str.compare("hard")==0) {
        return Source::Generator::hard;
    } else {
        throw std::logic_error("Unrecognized generator hardness: " + str);
    }
}

PhysicalModel::PhysicalModel::Type Parser::strToMaterialType(std::string str) 
{
    typedef SEMBA::PhysicalModel::PhysicalModel::Type Type;

    str = trim(str);
    if (str.compare("PEC")==0) {
        return Type::PEC;
    } else if (str.compare("PMC")==0) {
        return Type::PMC;
    } else if (str.compare("PML")==0) {
        return Type::PML;
    } else if (str.compare("SMA")==0) {
        return Type::SMA;
    } else if (str.compare("Classic")==0) {
        return Type::classic;
    } else if (str.compare("Dispersive")==0) {
        return Type::elecDispersive;
    } else if (str.compare("Anisotropic")==0) {
        return Type::anisotropic;
    } else if (str.compare("SIBC")==0) {
        return Type::isotropicsibc;
    } else if (str.compare("Wire")==0) {
        return Type::wire;
    } else if (str.compare("Connector")==0) {
        return Type::multiport;
    } else if (str.find("Thin_gap")==0) {
        return Type::gap;
    } else if (str.find("PriorityMaterial")==0) {
        return Type::priorityMaterial;
    } else {
        throw std::logic_error("Unrecognized material label: " + str);
    }
}

PhysicalModel::Multiport::Multiport::Type Parser::strToMultiportType(std::string str) {
    using namespace PhysicalModel::Multiport;

    str = trim(str);
    if (str.compare("Conn_short")==0) {
        return Multiport::Type::shortCircuit;
    } else if (str.compare("Conn_open")==0) {
        return Multiport::Type::openCircuit;
    } else if (str.compare("Conn_matched")==0) {
        return Multiport::Type::matched;
    } else if (str.compare("Conn_sRLC")==0) {
        return Multiport::Type::sRLC;
    } else if (str.compare("Conn_pRLC")==0) {
        return Multiport::Type::pRLC;
    } else if (str.compare("Conn_sLpRC")==0) {
        return Multiport::Type::sLpRC;
    } else if (str.compare("Conn_dispersive") == 0) {
        return Multiport::Type::dispersive;
    } else {
        throw std::logic_error("Unrecognized multiport label: " + str);
    }
}

std::pair<Math::CVecR3, Math::CVecR3> Parser::strToBox(
        const std::string& value) {
    std::size_t begin = value.find_first_of("{");
    std::size_t end = value.find_last_of("}");
    std::string aux = value.substr(begin+1,end-1);
    std::stringstream iss(aux);
    Math::CVecR3 max, min;
    for (std::size_t i = 0; i < 3; i++) {
        iss >> max(i);
    }
    for (std::size_t i = 0; i < 3; i++) {
        iss >> min(i);
    }
    return {min, max};
}

Math::CVecI3 Parser::strToCVecI3(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), '{'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '}'), str.end());
    std::stringstream ss(str);
    Math::CVecI3 res;
    ss >> res(Math::Constants::x)
       >> res(Math::Constants::y)
       >> res(Math::Constants::z);
    return res;
}

Math::CVecR3 Parser::strToCVecR3(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), '{'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '}'), str.end());
    std::stringstream ss(str);
    Math::CVecR3 res;
    ss >> res(Math::Constants::x)
       >> res(Math::Constants::y)
       >> res(Math::Constants::z);
    return res;
}

Source::OnLine::Type Parser::strToNodalType(std::string str) {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return Source::OnLine::Type::electric;
    } else if (str.compare("magneticField")==0) {
        return Source::OnLine::Type::magnetic;
    } else {
        throw std::logic_error("Unrecognized nodal type: " + str);
    }
}

Source::OnLine::Hardness Parser::strToNodalHardness(std::string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Source::OnLine::Hardness::soft;
    } else if (str.compare("hard")==0) {
        return Source::OnLine::Hardness::hard;
    } else {
        throw std::logic_error("Unrecognized nodal hardness: " + str);
    }
}

OutputRequest::Domain Parser::readDomain(const json& j) {
    bool timeDomain = false;
	Math::Real initialTime = 0.0;
	Math::Real finalTime = 0.0;
	Math::Real samplingPeriod = 0.0;

	bool frequencyDomain = false;
	bool logFrequencySweep = false;
	bool usingTransferFunction = false;
	Math::Real initialFrequency = 0.0;
	Math::Real finalFrequency = 0.0;
	Math::Real frequencyStep = 0.0;
    std::string transferFunctionFile;

    if (j.find("initialTime") != j.end()) {
        timeDomain     = true;
        initialTime    = j.at("initialTime").get<double>();
        finalTime      = j.at("finalTime").get<double>();
        samplingPeriod = j.at("samplingPeriod").get<double>();
    }

    if (j.find("initialFrequency") != j.end()) {
        frequencyDomain   = true;
        initialFrequency  = j.at("initialFrequency").get<double>();
        finalFrequency    = j.at("finalFrequency").get<double>();
        frequencyStep = getProgressionStepByTotalNumber(j, "Frequency");

        logFrequencySweep = j.at("logFrequencySweep").get<bool>();
        if (j.find("transferFunctionFile") != j.end()) {
            usingTransferFunction = true;
            transferFunctionFile = j.at(
                "transferFunctionFile"
            ).get<std::string>();
        }
    }

    return OutputRequest::Domain(
            timeDomain, initialTime, finalTime, samplingPeriod,
            frequencyDomain, initialFrequency, finalFrequency,
            frequencyStep, logFrequencySweep,
            usingTransferFunction, transferFunctionFile);
}

std::unique_ptr<Source::Magnitude::Magnitude> Parser::readMagnitude(const json& j) {
    std::string type = j.at("type").get<std::string>();
    if (type.compare("File") == 0) {
		return std::make_unique<Source::Magnitude::Numerical>(
			Source::Magnitude::Numerical(
				j.at("filename").get<std::string>())
		);
    }

    if (type.compare("Gaussian") == 0) {
		return std::make_unique<Source::Magnitude::Magnitude>(
			Source::Magnitude::Magnitude(
				new Math::Function::Gaussian(
					Math::Function::Gaussian::buildFromMaximumFrequency(
						j.at("frequencyMaximum").get<double>(),
						1.0
					)
				)
			)
		);
    }

    if (type.compare("Band_limited") == 0) {
		return std::make_unique<Source::Magnitude::Magnitude>(
			Source::Magnitude::Magnitude(
				new Math::Function::BandLimited(
					j.at("frequencyMinimum").get<double>(),
					j.at("frequencyMaximum").get<double>()))
		);
    }

    throw std::logic_error("Unable to recognize magnitude type when reading excitation.");
}

bool Parser::checkVersionCompatibility(const std::string& version) {
    bool versionMatches = (version == std::string(OPENSEMBA_VERSION));
    if (!versionMatches) {
        throw std::logic_error(
            "File version " + version + " is not supported.");
    }
    return versionMatches;
}

void Parser::checkExtendedVersionCompatibility(const std::string& version) {
	const char extended = version.back();

	if (isdigit(extended)) {
		throw std::logic_error(
			"File version " + version + " is not a valid extended version."
        );
	}

	// Check for extended availability
	if (version.substr(0, version.length() - 1) != std::string(OPENSEMBA_VERSION) || 
        extended != 'e') {
		throw std::logic_error(
			"File version " + version + " is not supported."
        );
	}
}

Math::Axis::Local Parser::strToLocalAxes(const std::string& str) {
    std::size_t begin = str.find_first_of("{");
    std::size_t end = str.find_first_of("}");
    Math::CVecR3 eulerAngles = strToCVecR3(str.substr(begin+1,end-1));
    begin = str.find_last_of("{");
    end = str.find_last_of("}");
    Math::CVecR3 origin = strToCVecR3(str.substr(begin+1,end-1));
    return Math::Axis::Local(eulerAngles, origin);
}

Source::Port::TEM::ExcitationMode Parser::strToTEMMode(std::string str) {
    if (str.compare("Voltage") == 0) {
        return Source::Port::TEM::voltage;
    } else if (str.compare("Current") == 0) {
        return Source::Port::TEM::current;
    } else {
        throw std::logic_error("Unrecognized exc. mode label: " + str);
    }

}

Source::Port::Waveguide::ExcitationMode Parser::strToWaveguideMode(
        std::string str) {
    if (str.compare("TE") == 0) {
        return Source::Port::Waveguide::ExcitationMode::TE;
    } else if (str.compare("TM") == 0) {
        return Source::Port::Waveguide::ExcitationMode::TM;
    } else {
        throw std::logic_error("Unrecognized exc. mode label: " + str);
    }
}

ElemView Parser::readCoordIdAsNodes(
    Mesh::Unstructured& mesh, 
    const json& j
) {
    ElemView res;
    
    for (auto it = j.begin(); it != j.end(); ++it) {
        const CoordR3* coord = mesh.coords().getId(CoordId(it->get<int>()));
        auto itCoord = mesh.elems().addAndAssignId(std::make_unique<NodR>(ElemId(0), &coord));

        res.push_back(itCoord->get());
    }

    return res;
}

Geometry::ElemView Parser::readElemIdsAsGroupOf(
    Geometry::Mesh::Unstructured& mesh,
    const Parser::json& j
) {
    Geometry::ElemView geometricElements;
    for (auto it = j.begin(); it != j.end(); ++it) {
        geometricElements.push_back(mesh.elems().getId(Geometry::ElemId(it->get<int>())));
    }
    return geometricElements;
}

} /* namespace JSON */
} /* namespace Parser */
} /* namespace SEMBA */
