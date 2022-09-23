#include "Exporter.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Tetrahedron4.h"
#include "geometry/mesh/Structured.h"
#include "geometry/mesh/Unstructured.h"
#include "geometry/mesh/Geometric.h"
#include "class/GroupViewTools.h"

namespace SEMBA {
namespace Exporters {
namespace GiD {
    
Math::Int Exporter::numberOfOutputGiD_ = 0;
Math::Int Exporter::coordCounter_ = 0;
Math::Int Exporter::elemCounter_ = 0;

const Math::CVecR3 pecColor(255, 0, 0);
const Math::CVecR3 pmcColor(0, 255, 0);
const Math::CVecR3 smaColor(0, 0, 255);
const Math::CVecR3 pmlColor(0, 0, 255);
const Math::CVecR3 sibcColor(100, 0, 100);
const Math::CVecR3 emSourceColor(100, 100, 0);

void Exporter::init_(
        const UnstructuredProblemDescription& smb,
        GiD_PostMode mode,
        const std::string& fn) {
    // Sets default values.
    mode_ = mode;
    meshFile_ = 0;
    resultFile_ = 0;
    if (numberOfOutputGiD_ == 0) {
        GiD_PostInit();
    }
    numberOfOutputGiD_++;
    // Opens files.
    deleteExistentOutputFiles();
    switch (mode_) {
    case GiD_PostAscii:
        openPostMeshFile_(fn + ".post.msh");
        break;
    case GiD_PostBinary:
        openPostResultFile_(fn + ".post.res");
        break;
    default:
        throw std::logic_error("Invalid GiD exporting mode.");
    }

    UnstructuredProblemDescription unstructuredProblemDescription(smb);
    auto& gSFactor = unstructuredProblemDescription.analysis.at("geometryScalingFactor");
    if (gSFactor != nullptr) {
        Math::Real scalingFactor = gSFactor.get<double>();
        if (scalingFactor != 0.0) {
            unstructuredProblemDescription.model.mesh.applyScalingFactor(1.0 / scalingFactor);
            unstructuredProblemDescription.grids.applyScalingFactor(1.0 / scalingFactor);
        }
    }

    writeMesh_(unstructuredProblemDescription);
}

Exporter::Exporter(const UnstructuredProblemDescription& smb, const std::string& fn, GiD_PostMode mode)
: 	SEMBA::Exporters::Exporter(fn) {
    init_(smb, mode, fn);
}

Exporter::~Exporter() {
    switch (mode_) {
    case GiD_PostAscii:
        GiD_fClosePostMeshFile(meshFile_);
        break;
    default:
        GiD_fClosePostResultFile(resultFile_);
    }
    if (numberOfOutputGiD_ == 1) {
        GiD_PostDone();
    }
    numberOfOutputGiD_--;
    GiD_fClosePostMeshFile(meshFile_);
}

void Exporter::writeAllElements_(
		const ElemRView& elems,
        const std::string& name) {
    writeElements_(
        Class::Group::View::filterView(
            elems,
            [](const Geometry::ElemR* elem) { return elem->is<Geometry::NodR>(); }
        ),
        name, 
        GiD_Point, 
        1
    );

    writeElements_(
        Class::Group::View::filterView(
            elems,
            [](const Geometry::ElemR* elem) { return elem->is<Geometry::LinR2>(); }
        ),
        name, 
        GiD_Linear, 
        2
    );

    writeElements_(
        Class::Group::View::filterView(
            elems,
            [](const Geometry::ElemR* elem) { return elem->is<Geometry::Tri3>(); }
        ),
        name, 
        GiD_Triangle,
        3
    );

    writeElements_(
        Class::Group::View::filterView(
            elems,
            [](const Geometry::ElemR* elem) { return elem->is<Geometry::QuaR4>(); }
        ),
        name, 
        GiD_Quadrilateral,
        4
    );

    writeElements_(
        Class::Group::View::filterView(
            elems,
            [](const Geometry::ElemR* elem) { return elem->is<Geometry::Tet4>(); }
        ),
        name, 
        GiD_Tetrahedra,
        4
    );
    
    writeElements_(
        Class::Group::View::filterView(
            elems,
            [](const Geometry::ElemR* elem) { return elem->is<Geometry::HexR8>(); }
        ),
        name, 
        GiD_Hexahedra,
        8
    );
}

ElemRView getElementsByTarget(const Geometry::ElemView& targets, const Geometry::Mesh::Unstructured* mesh) {
    ElemRView elementsToWrite;
    for (const auto& elem : mesh->elems()) {
        for (const auto& target : targets) {
            if (elem->getId() == target->getId()) {
                elementsToWrite.push_back(elem.get());
            }
        }
    }

    return elementsToWrite;
}

void Exporter::writeMesh_(const UnstructuredProblemDescription& smb)
{
    const Geometry::Mesh::Unstructured* inMesh = &smb.model.mesh;
    const SourceGroup& srcs = smb.sources;
    const OutputRequestGroup& oRqs = smb.outputRequests;

    const Geometry::Grid3* grid = nullptr;

    assert(inMesh != nullptr);
    const Geometry::Mesh::Unstructured* mesh;
    std::string preName;
    
    mesh = inMesh;
    grid = nullptr;

    // Writes materials.
    for (auto const& lay : mesh->layers()) {
        for (auto const& mat: smb.model.physicalModels) {
            writeAllElements_(
                mesh->elems().getMatLayerId(mat->getId(), lay->getId()),
                preName + mat->getName() + "@" + lay->getName()
            );
        }
    }

    // Writes EM Sources.
    for (const auto& src: srcs) {
        writeAllElements_(getElementsByTarget(src->getTarget(), mesh), preName + "EMSource_" + src->getName());
    }

    // Writes output requests.
    for (const auto& oRq: oRqs) {
        writeAllElements_(getElementsByTarget(oRq->getTarget(), mesh), preName + "OutRq_" + oRq->getName());
    }

    // Writes grid.
    Geometry::CoordR3Group cG;
    writeAllElements_(getGridElems(cG, grid).get(), "Grid");
    //
    GiD_FlushPostFile();
    //
    if (inMesh->is<Geometry::Mesh::Structured>()) {
        delete mesh;
    }
}

void Exporter::writeElements_(
        const ElemRView& elems,
		const std::string& name,
		const GiD_ElementType type,
		const Math::Int nV) {
    if (elems.size() == 0) {
        return;
    }
    Math::UInt tmpCounter = coordCounter_;
    std::vector<int> nId(nV);
    beginMesh(name, GiD_3D, type, nV);
    std::map<Geometry::CoordId, const Geometry::CoordR3*> pos;

    for(const auto& elem: elems) {
        for (Math::Int j = 0; j < nV; j++) {
            if (pos.count(elem->getVertex(j)->getId()) == 0) {
                pos[elem->getVertex(j)->getId()] = elem->getVertex(j);
            }
        }
    }

    Geometry::CoordR3Group cG;
    std::map<Geometry::CoordId, Math::UInt> mapCoords;
    for (auto const& entry: pos) {
        cG.add(std::make_unique<Geometry::CoordR3>(*entry.second));
        mapCoords[entry.first] = cG.size();
    }
    writeCoordinates_(cG);
    beginElements_();
    for (const auto& elem : elems) {
        for (Math::Int k = 0; k < nV; k++) {
            nId[k] = tmpCounter + mapCoords[elem->getVertex(k)->getId()];
        }
        writeElement_(++elemCounter_, &nId[0]);

    }
    endElements_();
    endMesh_();
}

void Exporter::beginMesh(
        const std::string& name,
        GiD_Dimension dim,
        GiD_ElementType elementType,
        Math::Int nNode,
        const Math::CVecR3& colorRGB) const {
    char *tName;
    tName = new char[name.length() + 1];
    strcpy(tName, name.c_str());
    if (colorRGB == Math::CVecR3(0.0, 0.0, 0.0)) {
        GiD_fBeginMesh(meshFile_, tName, dim, elementType, nNode);
    } else {
        GiD_fBeginMeshColor(meshFile_, tName, dim, elementType, nNode,
                (Math::Real) colorRGB(0),
				(Math::Real) colorRGB(1),
				(Math::Real) colorRGB(2));
    }
    delete [] tName;
}

void Exporter::beginResult(
        const std::string& fieldName,
        const std::string& timeName,
        const Math::Real time,
        GiD_ResultType resultType,
        GiD_ResultLocation resultLocaltion,
        const std::string gaussPointType,
        const std::vector<std::string>& componentsNames) const {
    std::vector<const char*> compv(componentsNames.size());
    for (std::size_t i = 0; i < componentsNames.size(); i++) {
        compv[i] = componentsNames[i].c_str();
    }
    GiD_fBeginResult(
        resultFile_, fieldName.c_str(), timeName.c_str(), time, resultType,
        getGiDResultLocation_(), gaussPointType.c_str(), nullptr,
        componentsNames.size(), &compv[0]);
}

void Exporter::openPostMeshFile_(
        const std::string& filename) {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    meshFile_ = GiD_fOpenPostMeshFile(auxChar, mode_);
    delete [] auxChar;
}

void Exporter::openPostResultFile_(
        const std::string& filename)  {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    resultFile_ = GiD_fOpenPostResultFile(auxChar, mode_);
    if (mode_ == GiD_PostBinary) {
        meshFile_ = resultFile_;
    }
    delete [] auxChar;
}

void Exporter::flushPostFile() const {
    if (meshFile_ != 0) {
        GiD_fFlushPostFile(meshFile_);
    }
    if (resultFile_ != 0) {
        GiD_fFlushPostFile(resultFile_);
    }
}

GiD_ResultType Exporter::getGiDResultType_(OutputRequest::OutputRequest::Type type) const {
    switch (type) {
    case OutputRequest::OutputRequest::Type::electric:
        return GiD_ResultType::GiD_Vector;
    case OutputRequest::OutputRequest::Type::magnetic:
        return GiD_ResultType::GiD_Vector;
    default:
        assert(false);
        return GiD_ResultType::GiD_Scalar;
    }
}

GiD_ResultLocation Exporter::getGiDResultLocation_() const {
    return GiD_ResultLocation::GiD_OnNodes;
}

void Exporter::writeCoordinates_(Geometry::CoordR3Group& cG)  {
    GiD_fBeginCoordinates(meshFile_);
    for (auto const& c: cG) {
        GiD_fWriteCoordinates(meshFile_, ++coordCounter_,
                (*c)(Math::Constants::x),
				(*c)(Math::Constants::y),
				(*c)(Math::Constants::z));
    }
    GiD_fEndCoordinates(meshFile_);
}

void Exporter::writeElement_(Math::Int elemId, int nId[]) const {
    GiD_fWriteElement(meshFile_, elemId, nId);
}

std::string Exporter::makeValid_(std::string name) {
    name.erase(remove(name.begin(), name.end(), '\n'), name.end());
    return name;
}

void Exporter::beginCoordinates_() const {
    GiD_fBeginCoordinates(meshFile_);
}

void Exporter::endCoordinates_() const {
    GiD_fEndCoordinates(meshFile_);
}

void Exporter::beginElements_() const {
    GiD_fBeginElements(meshFile_);
}

void Exporter::endElements_() const {
    GiD_fEndElements(meshFile_);
}

void Exporter::endMesh_() const {
    GiD_fEndMesh(meshFile_);
}

} /* namespace GiD */
} /* namespace Exporters */
} /* namespace SEMBA */
