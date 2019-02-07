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

#include "Exporter.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Tetrahedron4.h"
#include "geometry/mesh/Structured.h"
#include "geometry/mesh/Unstructured.h"

namespace SEMBA {
namespace Exporter {
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
        const Data* smb,
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
    writeMesh_(smb);
}

Exporter::Exporter(
        const Data* smb,
        const std::string& fn,
        GiD_PostMode mode)
: 	SEMBA::Exporter::Exporter(fn) {
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
		const Group::Group<const Geometry::ElemR>& elem,
        const std::string& name) {
    writeElements_(elem.getOf<Geometry::NodR>(), name, GiD_Point, 1);
    writeElements_(elem.getOf<Geometry::LinR2>(), name, GiD_Linear, 2);
    writeElements_(elem.getOf<Geometry::Tri3>() , name, GiD_Triangle, 3);
    writeElements_(elem.getOf<Geometry::QuaR4>(), name, GiD_Quadrilateral, 4);
    writeElements_(elem.getOf<Geometry::Tet4>(), name, GiD_Tetrahedra, 4);
    writeElements_(elem.getOf<Geometry::HexR8>(), name, GiD_Hexahedra, 8);
}

void Exporter::writeMesh_(const Data* smb) {
    const Geometry::Mesh::Mesh* inMesh = smb->mesh;
    const PhysicalModel::Group<>* mat = smb->physicalModels;
    const Source::Group<>* srcs = smb->sources;
    const OutputRequest::Group<>* oRqs = smb->outputRequests;
    const Geometry::Grid3* grid = nullptr;
    assert(inMesh != nullptr);
    assert(mat != nullptr);
    const Geometry::Mesh::Unstructured* mesh;
    std::string preName;
    if (inMesh->is<Geometry::Mesh::Structured>()) {

        mesh = inMesh->castTo<Geometry::Mesh::Structured>()->getMeshUnstructured();
        preName = "str_";
        grid = &inMesh->castTo<Geometry::Mesh::Structured>()->grid();
    } else {
        mesh = inMesh->castTo<Geometry::Mesh::Unstructured>();
        grid = nullptr;
    }
    // Writes materials.
    const Geometry::Layer::Group<>& lay = mesh->layers();
    for (std::size_t i = 0; i < lay.size(); i++) {
        for (std::size_t j = 0; j < mat->size(); j++) {
            const MatId matId = (*mat)(j)->getId();
            const Geometry::LayerId layId = lay(i)->getId();
            const std::string name =
            		preName + (*mat)(j)->getName() + "@" + lay(i)->getName();
            Group::Group<const Geometry::ElemR> elem =
            		mesh->elems().getMatLayerId(matId, layId);
            writeAllElements_(elem, name);
        }
    }
    // Writes EM Sources.
    if (srcs != nullptr) {
        for (std::size_t i = 0; i < srcs->size(); i++) {
            const Source::Base* src =  (*srcs)(i);
            const std::string name = preName + "EMSource_" + src->getName();
            Group::Group<const Geometry::ElemR> elem =
                    mesh->elems().getId(src->elems().getIds());
            writeAllElements_(elem, name);
        }
    }
    // Writes output requests.
    if (oRqs != nullptr) {
        for (std::size_t i = 0; i < oRqs->size(); i++) {
            const OutputRequest::Base* oRq = (*oRqs)(i);
            const std::string name = preName + "OutRq_" + oRq->getName();
            Group::Group<const Geometry::ElemR> elem =
                    mesh->elems().getId(oRq->elems().getIds());
            writeAllElements_(elem, name);
        }
    }
    // Writes boundaries.
    if (grid != nullptr) {
        for (Math::UInt i = 0; i < 3; i++) {
            for (Math::UInt j = 0; j < 2; j++) {
                Geometry::CoordR3Group cG;
                const Group::Group<Geometry::ElemR>& bound =
                        getBoundary(Math::Constants::CartesianAxis(i),
                        		    Math::Constants::CartesianBound(j),
                                    cG, grid, mesh);
                std::string name = getBoundaryName(
                		inMesh->castTo<Geometry::Mesh::Structured>(), i, j);
                writeAllElements_(bound, name);
            }
        }
    }
    // Writes grid.
    Geometry::CoordR3Group cG;
    Group::Group<Geometry::ElemR> gridAux = getGridElems(cG, grid);
    writeAllElements_(gridAux, "Grid");
    //
    GiD_FlushPostFile();
    //
    if (inMesh->is<Geometry::Mesh::Structured>()) {
        delete mesh;
    }
}

void Exporter::writeElements_(
		const Group::Group<const Geometry::ElemR>& elem,
		const std::string& name,
		const GiD_ElementType type,
		const Math::Int nV) {
    if (elem.size() == 0) {
        return;
    }
    Math::UInt tmpCounter = coordCounter_;
    std::vector<int> nId(nV);
    beginMesh(name, GiD_3D, type, nV);
    std::map<Geometry::CoordId, Geometry::CoordR3*> pos;
    for(std::size_t i = 0; i < elem.size(); i++) {
        for (Math::Int j = 0; j < nV; j++) {
            if (pos.count(elem(i)->getVertex(j)->getId()) == 0) {
                pos[elem(i)->getVertex(j)->getId()] =
                    elem(i)->getVertex(j)->clone();
            }
        }
    }
    Geometry::CoordR3Group cG;
    std::map<Geometry::CoordId, Math::UInt> mapCoords;
    for (std::map<Geometry::CoordId, Geometry::CoordR3*>::const_iterator
         it = pos.begin(); it != pos.end(); ++it) {
        cG.add(it->second);
        mapCoords[it->first] = cG.size();
    }
    writeCoordinates_(cG);
    beginElements_();
    for (std::size_t j = 0; j < elem.size(); j++) {
        for (Math::Int k = 0; k < nV; k++) {
            nId[k] = tmpCounter + mapCoords[elem(j)->getVertex(k)->getId()];
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

GiD_ResultType Exporter::getGiDResultType_(OutputRequest::Base::Type type) const {
    switch (type) {
    case OutputRequest::Base::electric:
        return GiD_ResultType::GiD_Vector;
    case OutputRequest::Base::magnetic:
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
    for (std::size_t i = 0; i < cG.size(); i++) {
        GiD_fWriteCoordinates(meshFile_, ++coordCounter_,
                (*cG(i))(Math::Constants::x),
				(*cG(i))(Math::Constants::y),
				(*cG(i))(Math::Constants::z));
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
} /* namespace Exporter */
} /* namespace SEMBA */
