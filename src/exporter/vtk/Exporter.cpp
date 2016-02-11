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

#include "geometry/mesh/Unstructured.h"
#include "geometry/element/Triangle.h"
#include "geometry/element/Quadrilateral.h"
#include "geometry/element/Tetrahedron.h"

namespace {
#ifdef _WIN32
    const std::string Separator = "\\";
#else
    const std::string Separator = "/";
#endif
}

namespace SEMBA {
namespace Exporter {
namespace VTK {

Exporter::Exporter(const Data* smb,
                   const std::string& fn)
:   SEMBA::Exporter::Exporter(fn) {
    initDir_(fn);
    writeMesh_(smb);
}

Exporter::~Exporter() {

}

void Exporter::writeMesh_(const Data* smb) {
    const Geometry::Mesh::Mesh* inMesh = smb->mesh;
    const PhysicalModel::Group<>* mat = smb->physicalModels;
    const Source::Group<>* srcs = smb->sources;
    const OutputRequest::Group<>* oRqs = smb->outputRequests;
    const Geometry::Grid3* grid = NULL;
    assert(inMesh != NULL);
    assert(mat != NULL);
    const Geometry::Mesh::Unstructured* mesh;
    const Geometry::Mesh::Structured* meshStr;
    std::string preName;
    if (inMesh->is<Geometry::Mesh::Structured>()) {
        meshStr = inMesh->castTo<Geometry::Mesh::Structured>();
        mesh = meshStr->getMeshUnstructured();
        preName = "str_";
        grid = inMesh->castTo<Geometry::Grid3>();
    } else {
        meshStr = NULL;
        mesh = inMesh->cloneTo<Geometry::Mesh::Unstructured>();
        grid = NULL;
    }

    std::string filename = getFilename() + ".pvd";
    std::ofstream outFile(filename.c_str());
    outFile << "<?xml version=\"1.0\"?>" << std::endl;
    outFile << "<VTKFile "
            << "type=\"Collection\" "
            << "version=\"0.1\" "
            << "byte_order=\"LittleEndian\" "
            << "compressor=\"vtkZLibDataCompressor\""
            << ">" << std::endl;
    outFile << "  " << "<Collection>" << std::endl;

    const Geometry::Layer::Group<Geometry::Layer::Layer>& lay = mesh->layers();
    std::size_t part = 0;
    // Writes materials.
    if (mat->size() > 0) {
        for (std::size_t i = 0; i < lay.size(); i++) {
            const Geometry::LayerId layId = lay(i)->getId();
            for (std::size_t j = 0; j < mat->size(); j++) {
                const MatId matId = (*mat)(j)->getId();
                const std::string name = preName + (*mat)(j)->getName() +
                                         "@" + lay(i)->getName();
                Group::Group<const Geometry::ElemR> elem = 
                    mesh->elems().getMatLayerId(matId, layId);
                writeFile_(elem, makeValid_(name), outFile, part);
            }
        }
    } else {
        for (std::size_t i = 0; i < lay.size(); i++) {
            const Geometry::LayerId layId = lay(i)->getId();
            Group::Group<const Geometry::ElemR> elem =
                mesh->elems().getLayerId(layId);
            const std::string name = preName + lay(i)->getName();
            writeFile_(elem, makeValid_(name), outFile, part);
        }
    }
    // Writes EM Sources.
    if (srcs != NULL) {
        for (std::size_t i = 0; i < srcs->size(); i++) {
            const Source::Base* src =  (*srcs)(i);
            const std::string name = preName + "EMSource_" + src->getName();
            Group::Group<const Geometry::ElemR> elem =
                mesh->elems().getId(src->elems().getIds());
            writeFile_(elem, makeValid_(name), outFile, part);
        }
    }
    // Writes output requests.
    if (oRqs != NULL) {
        for (std::size_t i = 0; i < oRqs->size(); i++) {
            const OutputRequest::Base* oRq = (*oRqs)(i);
            const std::string name = preName + "OutRq_" + oRq->getName();
            Group::Group<const Geometry::ElemR> elem =
                mesh->elems().getId(oRq->elems().getIds());
            writeFile_(elem, makeValid_(name), outFile, part);
        }
    }
    // Writes boundaries.
    if (meshStr != NULL && grid != NULL) {
        for (std::size_t i = 0; i < 3; i++) {
            for (std::size_t j = 0; j < 2; j++) {
                if (meshStr->bounds()(i, j) != NULL) {
                    Geometry::CoordR3Group cG;
                    Group::Group<Geometry::ElemR> bound =
                        getBoundary(Math::Constants::CartesianAxis(i),
                                    Math::Constants::CartesianBound(j),
                                    cG, grid, meshStr);
                    std::string name = getBoundaryName(meshStr, i, j);
                    writeFile_(bound, makeValid_(name), outFile, part);
                }
            }
        }
    }
    // Writes grid.
    if (grid != NULL) {
        Geometry::CoordR3Group cG;
        Group::Group<Geometry::ElemR> gridAux = getGridElems(cG, grid);
        writeFile_(gridAux, makeValid_("Grid"), outFile, part);
    }
    // Closes file.
    outFile << "  " << "</Collection>" << std::endl;
    outFile << "</VTKFile>" << std::endl;

    delete mesh;
}

void Exporter::writeFile_(const Group::Group<const Geometry::ElemR>& elems,
                          const std::string& name,
                          std::ofstream& outMain,
                          std::size_t& part) {
    if (elems.empty()) {
        return;
    }
    outMain << "    " << "<DataSet "
            << "group=\"" << name << "\" "
            << "part=\"" << part++ << "\" "
            << "file=\""
            << getBasename() + ".vtk" + Separator + name + ".vtu" << "\" "
            << "/>" << std::endl;

    std::string filename = getFilename() + ".vtk" + Separator + name + ".vtu";
    std::ofstream outFile(filename.c_str());
    outFile << "<VTKFile "
            << "type=\"UnstructuredGrid\" "
            << "version=\"0.1\" "
            << "byte_order=\"LittleEndian\""
            << ">" << std::endl;
    std::pair<std::vector<Math::CVecR3>,
              std::map<Geometry::CoordId, std::size_t>> aux =
        getPoints_(elems);
    outFile << "  " << "<UnstructuredGrid "
            << "NumberOfPoints=\"" << aux.first.size() << "\" "
            << "NumberOfCells=\"" << elems.size() << "\" "
            << ">" << std::endl;
    writePoints_(outFile, aux.first);
    writeCells_(outFile, elems, aux.second);
    outFile << "  " << "</UnstructuredGrid>" << std::endl;
    outFile << "</VTKFile>" << std::endl;
    outFile.close();
}

std::pair<std::vector<Math::CVecR3>, std::map<Geometry::CoordId, std::size_t>>
    Exporter::getPoints_(
        const Group::Group<const Geometry::ElemR>& elems) {
    std::map<Geometry::CoordId, std::size_t> mapCoords;
    std::vector<Math::CVecR3> pos;
    for (std::size_t i = 0; i < elems.size(); i++) {
        for (std::size_t j = 0; j < elems(i)->numberOfVertices(); j++) {
            if (mapCoords.count(elems(i)->getVertex(j)->getId()) == 0) {
                mapCoords[elems(i)->getVertex(j)->getId()] = pos.size();
                pos.push_back(elems(i)->getVertex(j)->pos());
            }
        }
    }
    return make_pair(pos, mapCoords);
}

void Exporter::writePoints_(std::ofstream &outFile,
                            const std::vector<Math::CVecR3>& pos) {
    outFile << "      " << "<Points>" << std::endl;
    outFile << "        " << "<DataArray "
            << "type=\"Float32\" "
            << "NumberOfComponents=\"3\" "
            << "format=\"ascii\""
            << ">" << std::endl;
    outFile << "         ";
    for (std::size_t i = 0; i < pos.size(); i++) {
        outFile << " " << pos[i](Math::Constants::x)
                << " " << pos[i](Math::Constants::y)
                << " " << pos[i](Math::Constants::z);
    }
    outFile << std::endl;
    outFile << "        " << "</DataArray>" << std::endl;
    outFile << "      " << "</Points>" << std::endl;
}

void Exporter::writeCells_(
        std::ofstream& outFile,
        const Group::Group<const Geometry::ElemR>& elems,
        std::map<Geometry::CoordId, std::size_t>& mapCoords) {

    outFile << "      " << "<Cells>" << std::endl;
    outFile << "        " << "<DataArray "
            << "type=\"Int32\" "
            << "Name=\"connectivity\" "
            << "format=\"ascii\""
            << ">" << std::endl;
    outFile << "         ";
    for (std::size_t i = 0; i < elems.size(); i++) {
        for (std::size_t j = 0; j < elems(i)->numberOfVertices(); j++) {
            outFile << " " << mapCoords.at(elems(i)->getVertex(j)->getId());
        }
    }
    outFile << std::endl;
    outFile << "        " << "</DataArray>" << std::endl;

    outFile << "        " << "<DataArray "
            << "type=\"Int32\" "
            << "Name=\"offsets\" "
            << "format=\"ascii\""
            << ">" << std::endl;
    outFile << "         ";
    std::size_t offset = 0;
    for (std::size_t i = 0; i < elems.size(); i++) {
        offset += elems(i)->numberOfVertices();
        outFile << " " << offset;
    }
    outFile << std::endl;
    outFile << "        " << "</DataArray>" << std::endl;

    outFile << "        " << "<DataArray "
            << "type=\"UInt8\" "
            << "Name=\"types\" "
            << "format=\"ascii\""
            << ">" << std::endl;
    outFile << "         ";
    for (std::size_t i = 0; i < elems.size(); i++) {
        outFile << " ";
        if (elems(i)->is<Geometry::Nod>()) {
            outFile << CELL_TYPES::VTK_VERTEX;
        } else if (elems(i)->is<Geometry::Lin>()) {
            outFile << CELL_TYPES::VTK_LINE;
        } else if (elems(i)->is<Geometry::Tri>()) {
            outFile << CELL_TYPES::VTK_TRIANGLE;
        } else if (elems(i)->is<Geometry::Qua>()) {
            outFile << CELL_TYPES::VTK_QUAD;
        } else if (elems(i)->is<Geometry::Tet>()) {
            outFile << CELL_TYPES::VTK_TETRA;
        } else if (elems(i)->is<Geometry::Hex8>()) {
            outFile << CELL_TYPES::VTK_HEXAHEDRON;
        } else {
            outFile << CELL_TYPES::VTK_POLY_LINE;
        }
    }
    outFile << std::endl;
    outFile << "        " << "</DataArray>" << std::endl;
    outFile << "      " << "</Cells>" << std::endl;
}

std::string Exporter::makeValid_(const std::string& allocator) {
    std::string res = allocator;
    std::replace(res.begin(), res.end(), '/', '_');
    return res;
}

} /* namespace VTK */
} /* namespace Exporter */
} /* namespace SEMBA */
