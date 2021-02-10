// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include "Parser.h"

#include "math/function/Gaussian.h"
#include "math/function/BandLimited.h"
#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Triangle6.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Tetrahedron4.h"
#include "geometry/element/Tetrahedron10.h"
#include "geometry/element/Hexahedron8.h"
#include "physicalModel/bound/Bound.h"
#include "physicalModel/multiport/Dispersive.h"
#include "physicalModel/multiport/Predefined.h"
#include "physicalModel/multiport/RLC.h"
#include "physicalModel/predefined/PEC.h"
#include "physicalModel/predefined/PMC.h"
#include "physicalModel/predefined/SMA.h"
#include "physicalModel/volume/AnisotropicCrystal.h"
#include "physicalModel/volume/AnisotropicFerrite.h"
#include "physicalModel/volume/Classic.h"
#include "physicalModel/volume/PML.h"
#include "physicalModel/wire/Wire.h"
#include "physicalModel/gap/Gap.h"
#include "source/port/WaveguideRectangular.h"
#include "source/port/TEMCoaxial.h"
#include "outputRequest/BulkCurrent.h"
#include "outputRequest/FarField.h"

namespace SEMBA {
namespace Parser {
namespace JSON {

Data Parser::read(std::istream& stream) const {

    json j;
    try {
        stream >> j;
    }
    catch( const std::exception & ex ) {
        std::cerr << ex.what() << std::endl;
    }

    Util::ProgressBar progress;
    progress.init("Parser GiD-JSON", 7, 0);

    std::string version = j.at("_version").get<std::string>();
    if (!checkVersionCompatibility(version)) {
        throw std::logic_error(
                "File version " + version + " is not supported.");
    }
    progress.advance();

    Data res;

    res.solver = readSolver(j);
    progress.advance();

    res.physicalModels = readPhysicalModels(j);
    progress.advance();

    res.mesh = readGeometricMesh(*res.physicalModels, j);
    progress.advance();

    if (res.mesh != nullptr) {
		readConnectorOnPoint(
			*res.physicalModels,
			*res.mesh->castTo<Geometry::Mesh::Geometric>(), j);
		progress.advance();

        res.sources = readSources(
                *res.mesh->castTo<Geometry::Mesh::Geometric>(), j);
        progress.advance();

        res.outputRequests = readOutputRequests(
                *res.mesh->castTo<Geometry::Mesh::Geometric>(), j);
        progress.advance();
    } else {
        res.sources = new Source::Group<>();
        res.outputRequests = new OutputRequest::Group<>();
    }

    postReadOperations(res);
    progress.advance();

    progress.end();

    return res;
}

Solver::Info* Parser::readSolver(const json& j) {
    if (j.find("solverOptions") == j.end()) {
        return nullptr;
    }
    json solverOptions = j.at("solverOptions").get<json>();
    Solver::Settings opts = readSolverSettings(solverOptions);
    return new Solver::Info(
            solverOptions.at("solver").get<std::string>(),
            std::move(opts));
}

Solver::Settings Parser::readSolverSettings(const json& j) {
    Solver::Settings opts;
    opts.setObject();
    for (json::const_iterator it = j.begin(); it != j.end(); ++it) {
        if (it->is_object()) {
            Solver::Settings aux = readSolverSettings(*it);
            opts.addMember(it.key(), std::move(aux));
        } else {
            Solver::Settings aux;
            std::stringstream value;
            if (it->type() == json::value_t::string) {
                value << it->get<std::string>();
            } else if (it->type() == json::value_t::boolean) {
				if (*it) {
					value << 1;
				} else {
					value << 0;
				}
			} else {
                value << *it;
            }
            aux.setString(value.str());
            opts.addMember(it.key(), std::move(aux));
        }
    }
    return opts;
}

Geometry::Mesh::Geometric* Parser::readGeometricMesh(
        const PhysicalModel::Group<>& physicalModels, const json& j) {
    try {
        Geometry::Grid3 grid = readGrids(j);
        Geometry::Layer::Group<> layers = readLayers(j);
        Geometry::Coordinate::Group<Geometry::CoordR3> coords = readCoordinates(j);
        Geometry::Element::Group<Geometry::ElemR> elements =
                readElements(physicalModels, layers, coords, j);
        return new Geometry::Mesh::Geometric(grid, coords, elements, layers);
    }
    catch (...) {
        return nullptr;
    }
}

Source::Group<>* Parser::readSources(
        Geometry::Mesh::Geometric& mesh, const json& j) {
    Source::Group<>* res = new Source::Group<>();

    const json& sources = j.at("sources").get<json>();
    for (json::const_iterator it = sources.begin(); it != sources.end(); ++it) {
        std::string sourceType = it->at("sourceType").get<std::string>();
        if (sourceType.compare("planewave") == 0) {
            res->add(readPlanewave(mesh, *it));
        } else if (sourceType.compare("generator") == 0) {
            res->add(readGenerator(mesh, *it));
        } else if (sourceType.compare("sourceOnLine") == 0) {
            res->add(readSourceOnLine(mesh, *it));
        } else if (sourceType.compare("waveguidePort") == 0) {
            res->add(readPortWaveguide(mesh, *it));
        } else if (sourceType.compare("temPort") == 0) {
            res->add(readPortTEM(mesh, *it));
        } else {
            throw std::logic_error("Unrecognized source type: " + sourceType);
        }
    }
    return res;
}

PhysicalModel::Group<>* Parser::readPhysicalModels(const json& j){
    if (j.find("materials") == j.end()) {
        return nullptr;
    }
    PhysicalModel::Group<>* res = new PhysicalModel::Group<>();
    json mats = j.at("materials");
    for (json::const_iterator it = mats.begin(); it != mats.end(); ++it) {
        res->add(readPhysicalModel( *it ));
    }
    return res;
}

PhysicalModel::PhysicalModel* Parser::readPhysicalModel(const json& j) {
    PhysicalModel::PhysicalModel::Type type =
                strToMaterialType( j.at("materialType").get<std::string>() );
	MatId id;
	if (j.find("materialId") != j.end()) {
		id = MatId(j.at("materialId").get<int>());
	} else {
		id = MatId(0);
	}
	std::string name;
	if (j.find("name") != j.end()) {
		name = j.at("name").get<std::string>();
	}

    switch (type) {
    case PhysicalModel::PhysicalModel::PEC:
        return new PhysicalModel::Predefined::PEC (id, name);

    case PhysicalModel::PhysicalModel::PMC:
        return new PhysicalModel::Predefined::PMC(id, name);

    case PhysicalModel::PhysicalModel::SMA:
        return new PhysicalModel::Predefined::SMA(id, name);

    case PhysicalModel::PhysicalModel::PML:
        return new PhysicalModel::Volume::PML(id, name, 
			strToLocalAxes(j.at("localAxes").get<std::string>()));

    case PhysicalModel::PhysicalModel::classic:
        return new PhysicalModel::Volume::Classic(id, name,
                j.at("permittivity").get<double>(),
                j.at("permeability").get<double>(),
                j.at("electricConductivity").get<double>(),
                j.at("magneticConductivity").get<double>());

    case PhysicalModel::PhysicalModel::elecDispersive:
        return new PhysicalModel::Volume::Dispersive(id, name,
                j.at("filename").get<std::string>());

    case PhysicalModel::PhysicalModel::wire:
    {
        std::string wireType = j.at("wireType").get<std::string>();
        if (wireType.compare("Dispersive") == 0) {
            return new PhysicalModel::Wire::Wire(id, name,
                    j.at("radius").get<double>(),
                    j.at("filename").get<std::string>());
        } else if(wireType.compare("SeriesParallel") == 0) {
            return new PhysicalModel::Wire::Wire(id, name,
                    j.at("radius").get<double>(),
                    j.at("resistance").get<double>(),
                    j.at("inductance").get<double>(),
                    j.at("capacitance").get<double>(),
                    j.at("parallelResistance").get<double>(),
                    j.at("parallelInductance").get<double>(),
                    j.at("parallelCapacitance").get<double>());
        } else if(wireType.compare("Standard") == 0) {
            return new PhysicalModel::Wire::Wire(id, name,
                    j.at("radius").get<double>(),
                    j.at("resistance").get<double>(),
                    j.at("inductance").get<double>());
        } else {
            throw std::logic_error("Unrecognized wire type" + wireType);
        }
    }

    case PhysicalModel::PhysicalModel::anisotropic:
    {
        std::string str = j.at("anisotropicModel").get<std::string>();
        if (str.compare("Crystal")==0) {
            return new PhysicalModel::Volume::AnisotropicCrystal(id, name,
                    strToLocalAxes(j.at("localAxes").get<std::string>()),
                    strToCVecR3(
                            j.at("relativePermittiviy").get<std::string>()),
                    j.at("crystalRelativePermeability").get<double>());
        } else if (str.compare("Ferrite")==0) {
            return new PhysicalModel::Volume::AnisotropicFerrite(id, name,
                    strToLocalAxes(j.at("localAxes").get<std::string>()),
                    j.at("kappa").get<double>(),
                    j.at("ferriteRelativePermeability").get<double>(),
                    j.at("ferriteRelativePermittivity").get<double>());
        } else {
            throw std::logic_error("Unrecognized Anisotropic Model: " + str);
        }
    }

    case PhysicalModel::PhysicalModel::isotropicsibc:
    {
        std::string sibcType = j.at("surfaceType").get<std::string>();
        if (sibcType.compare("File")==0) {
            return new PhysicalModel::Surface::SIBCFile(id, name,
                    j.at("filename").get<std::string>() );
        } else if (sibcType.compare("Layers")==0) {
            return readMultilayerSurface(j);
        } else {
            throw std::logic_error("Unrecognized SIBC type: " + sibcType);
        }
    }

    case PhysicalModel::PhysicalModel::gap:
        return new PhysicalModel::Gap::Gap(id, name,
                j.at("width").get<double>());

    case PhysicalModel::PhysicalModel::multiport:
    {
        PhysicalModel::Multiport::Multiport::Type mpType =
                strToMultiportType(j.at("connectorType").get<std::string>());
		if (mpType == PhysicalModel::Multiport::Multiport::shortCircuit) {
			return new PhysicalModel::Multiport::Predefined(id, name, mpType);
		} else if (mpType == PhysicalModel::Multiport::Multiport::openCircuit) {
			return new PhysicalModel::Multiport::Predefined(id, name, mpType);
        } else if (mpType == PhysicalModel::Multiport::Multiport::dispersive) {
            return new PhysicalModel::Multiport::Dispersive(id, name,
                    j.at("filename").get<std::string>());
        } else {
            return new PhysicalModel::Multiport::RLC(id, name, mpType,
                    j.at("resistance").get<double>(),
                    j.at("inductance").get<double>(),
                    j.at("capacitance").get<double>());
        }
    }

    default:
        throw std::logic_error("Material type not recognized for: " + name);
    }
}

OutputRequest::Group<>* Parser::readOutputRequests(
        Geometry::Mesh::Geometric& mesh, const json& j) {

    OutputRequest::Group<>* res = new OutputRequest::Group<>();
    const json& outs = j.at("outputRequests").get<json>();
    for (json::const_iterator it = outs.begin(); it != outs.end(); ++it) {
        res->add(readOutputRequest(mesh, *it));
    }
    return res;
}

void Parser::readConnectorOnPoint(
	PMGroup& pMG,
	Geometry::Mesh::Geometric& mesh, 
	const json& j) {
	const json& conns = j.at("connectorOnPoint").get<json>();
	for (auto it = conns.begin(); it != conns.end(); it++) {
		PhysicalModel::PhysicalModel* mat = readPhysicalModel(*it);
		pMG.addId(mat);
				
		Geometry::CoordId cId( it->at("coordIds").get<int>() );
		const Geometry::CoordR3* coord[1] = { mesh.coords().getId(cId) };
		Geometry::ElemId eId(0);
		Geometry::NodR* node = 
			new Geometry::NodR(eId, coord, nullptr, pMG.getId(mat->getId()));
		
		mesh.elems().addId(node);
	}
}

Geometry::Element::Group<> Parser::boxToElemGroup(
        Geometry::Mesh::Geometric& mesh,
        const std::string& line) {
    Geometry::BoxR3 box = strToBox(line);
    if (box.isVolume()) {
        return mesh.elems().addId(
			new Geometry::HexR8(mesh.coords(), Geometry::ElemId(0), box));
    } else if (box.isSurface()) {
        return mesh.elems().addId(
			new Geometry::QuaR4(mesh.coords(), Geometry::ElemId(0), box));
    } else if (box.isLine()) {
		return mesh.elems().addId(
			new Geometry::LinR2(mesh.coords(), Geometry::ElemId(0), box));
	} else if (box.isPoint()) {
		return mesh.elems().addId(
			new Geometry::NodR(mesh.coords(), Geometry::ElemId(0), box));
	} else {
        throw std::logic_error(
                "Box to Elem Group only works for volumes and surfaces");
    }
}

OutputRequest::Base* Parser::readOutputRequest(
            Geometry::Mesh::Geometric& mesh, const json& j) {

    std::string name = j.at("name").get<std::string>();
    OutputRequest::Base::Type type =
            strToOutputType(j.at("type").get<std::string>());
    std::string gidOutputType = j.at("gidOutputType").get<std::string>();
    OutputRequest::Domain domain = readDomain(j.at("domain").get<json>());

	if (type == OutputRequest::Base::Type::bulkCurrentElectric ||
		type == OutputRequest::Base::Type::bulkCurrentMagnetic) {
		if (gidOutputType.compare("OutRq_on_layer") == 0) {
			return new OutputRequest::BulkCurrent(domain, name,
				boxToElemGroup(mesh, j.at("box").get<std::string>()),
				strToCartesianAxis(j.at("direction").get<std::string>()),
				j.at("skip").get<int>());
		} else if (gidOutputType.compare("OutRq_on_point") == 0) {
			return new OutputRequest::BulkCurrent(domain, name,
				readCoordIdAsNodes(mesh, j.at("elemIds").get<json>()),
				strToCartesianAxis(j.at("direction").get<std::string>()),
				j.at("skip").get<int>());
		} else {
			return new OutputRequest::BulkCurrent(domain, name,
				readElemIdsAsGroupOf<const Geometry::Elem>(
					mesh, j.at("elemIds").get<json>()),
				strToCartesianAxis(j.at("direction").get<std::string>()),
				j.at("skip").get<int>());
		}
	}

	if (gidOutputType.compare("OutRq_on_point") == 0) {
		return new OutRqElem(domain, type, name,
			readCoordIdAsNodes(mesh, j.at("elemIds").get<json>()));
	} else if (gidOutputType.compare("OutRq_on_line") == 0) {
		return new OutRqElem(domain, type, name,
			readElemIdsAsGroupOf<Geometry::Lin>(
				mesh, j.at("elemIds").get<json>()));
	} else if (gidOutputType.compare("OutRq_on_surface") == 0) {
		return new OutRqElem(domain, type, name,
        		readElemIdsAsGroupOf<Geometry::Surf>(
                		mesh, j.at("elemIds").get<json>()));
	} else if (gidOutputType.compare("OutRq_on_layer") == 0) {
        return new OutRqElem(domain, type, name, 
			boxToElemGroup(mesh, j.at("box").get<std::string>()));
	} else if (gidOutputType.compare("Far_field") == 0) {
		static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
		return new OutputRequest::FarField(domain, name,
			boxToElemGroup(mesh, j.at("box").get<std::string>()),
			j.at("initialTheta").get<double>() * degToRad,
			j.at("finalTheta").get<double>()   * degToRad,
			j.at("stepTheta").get<double>()    * degToRad,
			j.at("initialPhi").get<double>()   * degToRad,
			j.at("finalPhi").get<double>()     * degToRad,
			j.at("stepPhi").get<double>()      * degToRad);
    } else {
        throw std::logic_error(
                "Unrecognized GiD Output request type: " + gidOutputType);
    }
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

Geometry::Layer::Group<> Parser::readLayers(const json& j) {
    if (j.find("layers") == j.end()) {
        throw std::logic_error("layers object was not found.");
    }

    Geometry::Layer::Group<> res;
    const json layers = j.at("layers");
    for (json::const_iterator it = layers.begin(); it != layers.end(); ++it) {
        res.add(new Geometry::Layer::Layer(
                Geometry::Layer::Id(it->at("id").get<int>()),
                it->at("name").get<std::string>()));
    }
    return res;
}

Geometry::Coordinate::Group<Geometry::CoordR3> Parser::readCoordinates(
        const json& j) {

    if (j.find("coordinates") == j.end()) {
        throw std::logic_error("Coordinates label was not found.");
    }

    Geometry::Coordinate::Group<Geometry::CoordR3> res;
    const json& c = j.at("coordinates").get<json>();
    for (json::const_iterator it = c.begin(); it != c.end(); ++it) {
        Geometry::CoordId id;
        Math::CVecR3 pos;
        std::stringstream ss(it->get<std::string>());
        ss >> id >> pos(0) >> pos(1) >> pos(2);
        res.add(new Geometry::CoordR3(id, pos));
    }
    return res;
}

Geometry::Element::Group<Geometry::ElemR> Parser::readElements(
        const PhysicalModel::Group<>& mG,
        const Geometry::Layer::Group<>& lG,
        const Geometry::CoordR3Group& cG,
        const json& j) {

    if (j.find("elements") == j.end()) {
        throw std::logic_error("Elements label was not found.");
    }

    Geometry::Element::Group<Geometry::ElemR> res;
    const json& elems = j.at("elements").get<json>();


    res.add(readElemStrAs<Geometry::HexR8>(mG, lG, cG, elems.at("hexahedra").get<json>()));
    res.add(readElemStrAs<Geometry::Tet4> (mG, lG, cG, elems.at("tetrahedra").get<json>()));
    res.add(readElemStrAs<Geometry::QuaR4>(mG, lG, cG, elems.at("quadrilateral").get<json>()));
    res.add(readElemStrAs<Geometry::Tri3> (mG, lG, cG, elems.at("triangle").get<json>()));
    res.add(readElemStrAs<Geometry::LinR2>(mG, lG, cG, elems.at("line").get<json>()));

    return res;
}

PhysicalModel::Surface::Multilayer* Parser::readMultilayerSurface(
        const json& mat) {
    MatId id = MatId(mat.at("materialId").get<int>());
    std::string name = mat.at("name").get<std::string>();

    std::vector<PhysicalModel::Surface::Multilayer::Layer> layers;
    for (json::const_iterator it = mat.at("layers").begin();
            it != mat.at("layers").end(); ++it) {
        layers.push_back(
                PhysicalModel::Surface::Multilayer::Layer(
                        it->at("thickness").get<double>(),
                        it->at("permittivity").get<double>(),
                        it->at("permeability").get<double>(),
                        it->at("elecCond").get<double>() ));
    }

    if (mat.at("useSembaVectorFitting").get<bool>()) {
        PhysicalModel::Surface::Multilayer::FittingOptions opts(
                std::make_pair(mat.at("freqMin").get<double>(),
                        mat.at("freqMax").get<double>()),
                        mat.at("numberOfPoles").get<int>());
        return new PhysicalModel::Surface::Multilayer(id, name, layers, {opts});
    } else {
        return new PhysicalModel::Surface::Multilayer(id, name, layers);
    }
}

Geometry::Grid3 Parser::readGrids(const json& j) {
    if (j.find("grids") == j.end()) {
        throw std::logic_error("Grids object not found.");
    }

    json g = j.at("grids").front();
    std::string gridType = g.at("gridType").get<std::string>();
    if (gridType.compare("gridCondition") == 0) {
        // Initializes basic grid.
        Geometry::Grid3 res;
        if (g.at("type").get<std::string>().compare("Number_of_cells") == 0) {
            res = Geometry::Grid3(
                    strToBox(g.at("layerBox").get<std::string>()),
                    strToCVecI3(g.at("numberOfCells").get<std::string>()));
        } else {
            Geometry::BoxR3 box = strToBox(g.at("layerBox").get<std::string>());
            Math::CVecR3 stepSize = 
                strToCVecR3(g.at("stepSize").get<std::string>());
            if (g.at("fitSizeToBox").get<bool>()) {
                for (std::size_t i = 0; i < 3; i++) {
					std::size_t n = std::round(box.getLength()[i] / stepSize[i]);
					if (n == 0) {
						n = 1;
					}
                    stepSize[i] = box.getLength()[i] / n;
                }
            }
            res = Geometry::Grid3(box, stepSize);
        }

        // Applies boundary padding operations.
        if (g.find("lowerPaddingMeshSize") != g.end()) {
            std::pair<Math::CVecR3, Math::CVecR3> boundaryMeshSize(
                    strToCVecR3(g.at("lowerPaddingMeshSize").get<std::string>()),
                    strToCVecR3(g.at("upperPaddingMeshSize").get<std::string>()));
            std::pair<Math::CVecR3, Math::CVecR3> boundaryPadding(
                    strToCVecR3(g.at("lowerPadding").get<std::string>()),
                    strToCVecR3(g.at("upperPadding").get<std::string>()));
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
		Math::CVecR3 corner = 
			strToCVecR3(g.at("corner").get<std::string>());
		Math::CVecR3 boxSize = 
			strToCVecR3(g.at("boxSize").get<std::string>());
		Math::CVecI3 nGridPoints = 
			strToCVecI3(g.at("nGridPoints").get<std::string>());
        std::vector<Math::Real> pos[3];
        pos[0] = g.at("xCoordinates").get<std::vector<double>>();
        pos[1] = g.at("yCoordinates").get<std::vector<double>>();
        pos[2] = g.at("zCoordinates").get<std::vector<double>>();
		if (!pos[0].empty()) {
			return Geometry::Grid3(pos);
		} else {
			std::pair<Math::CVecR3, Math::CVecR3> box =
				{ corner, corner + boxSize };
			return Geometry::Grid3(box, nGridPoints);
		}

    } else {
        throw std::logic_error("Unrecognized grid type: " + gridType);
    }
}

Source::PlaneWave* Parser::readPlanewave(
        Geometry::Mesh::Geometric& mesh, const json& j) {

    Source::Magnitude::Magnitude* magnitude =
            readMagnitude(j.at("magnitude").get<json>());
	auto elems = boxToElemGroup(mesh, j.at("layerBox").get<std::string>());
	if (elems.sizeOf<Geometry::Vol>() == 0) {
		throw std::logic_error("Plane wave layer must define a volume");
	}
    
    std::string definitionMode = j.at("definitionMode").get<std::string>();
    if (definitionMode.compare("by_vectors")==0) {
        Math::CVecR3 dir =
                strToCVecR3(j.at("directionVector").get<std::string>());
        Math::CVecR3 pol =
                strToCVecR3(j.at("polarizationVector").get<std::string>());
        return new Source::PlaneWave(magnitude, elems, dir, pol);

    } else if (definitionMode.compare("by_angles")==0) {
        static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
        std::pair<Math::Real,Math::Real> dirAngles, polAngles;
        dirAngles.first  = j.at("directionTheta").get<double>()    * degToRad;
        dirAngles.second = j.at("directionPhi").get<double>()      * degToRad;
        polAngles.first  = j.at("polarizationAlpha").get<double>() * degToRad;
        polAngles.second = j.at("polarizationBeta").get<double>()  * degToRad;
        return new Source::PlaneWave(magnitude, elems, dirAngles, polAngles);

    } else if (definitionMode.compare("randomized_multisource")==0) {
        return new Source::PlaneWave(magnitude, elems,
                j.at("numberOfRandomPlanewaves").get<int>(),
                j.at("relativeVariationOfRandomDelay").get<double>());

    } else {
        throw std::logic_error("Unrecognized label: " + definitionMode);
    }
}

Source::Port::Waveguide* Parser::readPortWaveguide(
        Geometry::Mesh::Geometric& mesh, const json& j) {
    std::string shape = j.at("shape").get<std::string>();
    if (shape.compare("Rectangular") == 0) {
        return new Source::Port::WaveguideRectangular(
                readMagnitude(       j.at("magnitude").get<json>()),
                readElemIdsAsGroupOf<const Geometry::Surf>(
                             mesh, j.at("elemIds").get<json>()),
                strToWaveguideMode(  j.at("excitationMode").get<std::string>()),
                {j.at("firstMode").get<int>(), j.at("secondMode").get<int>()} );
    } else {
        throw std::logic_error("Unrecognized waveguide port shape: " + shape);
    }
}

Source::Port::TEM* Parser::readPortTEM(
        Geometry::Mesh::Geometric& mesh, const json& j) {
    return new Source::Port::TEMCoaxial(
            readMagnitude(       j.at("magnitude").get<json>()),
            readElemIdsAsGroupOf<const Geometry::Surf>(mesh, j.at("elemIds").get<json>()),
            strToTEMMode(        j.at("excitationMode").get<std::string>()),
            strToCVecR3(         j.at("origin").get<std::string>()),
                                 j.at("innerRadius").get<double>(),
                                 j.at("outerRadius").get<double>());
}

Source::Generator* Parser::readGenerator(
        Geometry::Mesh::Geometric& mesh, const json& j) {
	return new Source::Generator(
			readMagnitude(j.at("magnitude").get<json>()),
			readCoordIdAsNodes(mesh, j.at("coordIds").get<json>()),
			strToGeneratorType(j.at("type").get<std::string>()),
			Source::Generator::soft);
            //strToGeneratorHardness(j.at("hardness").get<std::string>()) );
}

Source::OnLine* Parser::readSourceOnLine(
        Geometry::Mesh::Geometric& mesh, const json& j) {
    return new Source::OnLine(
            readMagnitude(     j.at("magnitude").get<json>()),
			readElemIdsAsGroupOf<const Geometry::Lin>(
					mesh, j.at("elemIds").get<json>()),
            strToNodalType(    j.at("type").get<std::string>()),
            strToNodalHardness(j.at("hardness").get<std::string>()) );
}

OutputRequest::Base::Type Parser::strToOutputType(std::string str) {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return OutputRequest::Base::electric;
    } else if (str.compare("magneticField")==0) {
        return OutputRequest::Base::magnetic;
    } else if (str.compare("electricFieldNormals")==0) {
        return OutputRequest::Base::electricFieldNormals;
    } else if (str.compare("magneticFieldNormals")==0) {
        return OutputRequest::Base::magneticFieldNormals;
    } else if (str.compare("current")==0) {
        return OutputRequest::Base::current;;
    } else if (str.compare("bulkCurrentElectric")==0) {
        return OutputRequest::Base::bulkCurrentElectric;
    } else if (str.compare("bulkCurrentMagnetic")==0) {
        return OutputRequest::Base::bulkCurrentMagnetic;
	} else if (str.compare("surfaceCurrentDensity") == 0) {
		return OutputRequest::Base::surfaceCurrentDensity;
    } else if (str.compare("farField")==0) {
        return OutputRequest::Base::electric;
    } else {
        throw std::logic_error("Unrecognized output type: " + str);
    }
}

Source::Generator::Type Parser::strToGeneratorType(std::string str) {
    str = trim(str);
    if (str.compare("voltage")==0) {
        return Source::Generator::voltage;
    } else if (str.compare("current")==0) {
        return Source::Generator::current;
    } else {
        throw std::logic_error("Unrecognized generator type: " + str);
    }
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

PhysicalModel::PhysicalModel::Type Parser::strToMaterialType(std::string str) {
    str = trim(str);
    if (str.compare("PEC")==0) {
        return PhysicalModel::PhysicalModel::PEC;
    } else if (str.compare("PMC")==0) {
        return PhysicalModel::PhysicalModel::PMC;
    } else if (str.compare("PML")==0) {
        return PhysicalModel::PhysicalModel::PML;
    } else if (str.compare("SMA")==0) {
        return PhysicalModel::PhysicalModel::SMA;
    } else if (str.compare("Classic")==0) {
        return PhysicalModel::PhysicalModel::classic;
    } else if (str.compare("Dispersive")==0) {
        return PhysicalModel::PhysicalModel::elecDispersive;
    } else if (str.compare("Anisotropic")==0) {
        return PhysicalModel::PhysicalModel::anisotropic;
    } else if (str.compare("SIBC")==0) {
        return PhysicalModel::PhysicalModel::isotropicsibc;
    } else if (str.compare("Wire")==0) {
        return PhysicalModel::PhysicalModel::wire;
    } else if (str.compare("Connector")==0) {
        return PhysicalModel::PhysicalModel::multiport;
    } else if (str.find("Thin_gap")==0) {
        return PhysicalModel::PhysicalModel::gap;
    } else {
        throw std::logic_error("Unrecognized material label: " + str);
    }
}

PhysicalModel::Multiport::Multiport::Type Parser::strToMultiportType(
        std::string str) {
    str = trim(str);
    if (str.compare("Conn_short")==0) {
        return PhysicalModel::Multiport::Multiport::shortCircuit;
    } else if (str.compare("Conn_open")==0) {
        return PhysicalModel::Multiport::Multiport::openCircuit;
    } else if (str.compare("Conn_matched")==0) {
        return PhysicalModel::Multiport::Multiport::matched;
    } else if (str.compare("Conn_sRLC")==0) {
        return PhysicalModel::Multiport::Multiport::sRLC;
    } else if (str.compare("Conn_pRLC")==0) {
        return PhysicalModel::Multiport::Multiport::pRLC;
    } else if (str.compare("Conn_sLpRC")==0) {
        return PhysicalModel::Multiport::Multiport::sLpRC;
    } else if (str.compare("Conn_dispersive") == 0) {
        return PhysicalModel::Multiport::Multiport::dispersive;
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
        return Source::OnLine::electric;
    } else if (str.compare("magneticField")==0) {
        return Source::OnLine::magnetic;
    } else {
        throw std::logic_error("Unrecognized nodal type: " + str);
    }
}

Source::OnLine::Hardness Parser::strToNodalHardness(std::string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Source::OnLine::soft;
    } else if (str.compare("hard")==0) {
        return Source::OnLine::hard;
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
        frequencyStep     = j.at("frequencyStep").get<double>();
        logFrequencySweep = j.at("logFrequencySweep").get<bool>();
        if (j.find("transferFunctionFile") != j.end()) {
            usingTransferFunction = true;
            transferFunctionFile =
                    j.at("transferFunctionFile").get<std::string>();
        }
    }

    return OutputRequest::Domain(
            timeDomain, initialTime, finalTime, samplingPeriod,
            frequencyDomain, initialFrequency, finalFrequency,
            frequencyStep, logFrequencySweep,
            usingTransferFunction, transferFunctionFile);
}

Source::Magnitude::Magnitude* Parser::readMagnitude(const json& j) {
    std::string type = j.at("type").get<std::string>();
    if (type.compare("File") == 0) {
       return new Source::Magnitude::Numerical(
               j.at("filename").get<std::string>());
    } else if (type.compare("Gaussian") == 0) {
       return new Source::Magnitude::Magnitude(
           new Math::Function::Gaussian(
              Math::Function::Gaussian::buildFromMaximumFrequency(
                  j.at("frequencyMaximum").get<double>()
              )
           )
       );
    } else if (type.compare("Band_limited") == 0) {
        return new Source::Magnitude::Magnitude(
            new Math::Function::BandLimited(
                       j.at("frequencyMinimum").get<double>(),
                       j.at("frequencyMaximum").get<double>()));
    } else {
        throw std::logic_error(
            "Unable to recognize magnitude type when reading excitation.");
    }
}

bool Parser::checkVersionCompatibility(const std::string& version) {
    bool versionMatches = (version == std::string(OPENSEMBA_VERSION));
    if (!versionMatches) {
        throw std::logic_error(
                "File version " + version + " is not supported.");
    }
    return versionMatches;
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
        return Source::Port::Waveguide::TE;
    } else if (str.compare("TM") == 0) {
        return Source::Port::Waveguide::TM;
    } else {
        throw std::logic_error("Unrecognized exc. mode label: " + str);
    }
}

const PhysicalModel::Bound::Bound* Parser::strToBoundType(std::string str) {
    if (str.compare("PEC") == 0) {
        return new PhysicalModel::Bound::PEC(MatId(0));
    } else if (str.compare("PMC") == 0) {
        return new PhysicalModel::Bound::PMC(MatId(0));
    } else if (str.compare("PML") == 0) {
        return new PhysicalModel::Bound::PML(MatId(0));
    } else if (str.compare("Periodic") == 0) {
        return new PhysicalModel::Bound::Periodic(MatId(0));
    } else if (str.compare("MUR1") == 0) {
        return new PhysicalModel::Bound::Mur1(MatId(0));
    } else if (str.compare("MUR2") == 0) {
        return new PhysicalModel::Bound::Mur2(MatId(0));
    } else {
        throw std::logic_error("Unrecognized bound label: " + str);
    }
}

Geometry::Element::Group<Geometry::Nod> Parser::readCoordIdAsNodes(
        Geometry::Mesh::Geometric& mesh, const json& j) {
    std::vector<Geometry::ElemId> nodeIds;
    for (auto it = j.begin(); it != j.end(); ++it) {
        Geometry::CoordId coordId( it->get<int>() );
        const Geometry::CoordR3* coord = mesh.coords().getId(coordId);
        Geometry::NodR* node = new Geometry::NodR(Geometry::ElemId(0), &coord);
        mesh.elems().addId(node);
        nodeIds.push_back(node->getId());
    }
    return mesh.elems().getId(nodeIds);
}

} /* namespace JSON */
} /* namespace Parser */
} /* namespace SEMBA */
