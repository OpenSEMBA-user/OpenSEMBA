#include "Exporter.h"

#include "geometry/mesh/Unstructured.h"
#include "geometry/mesh/Geometric.h"

#include "geometry/element/Triangle.h"
#include "geometry/element/Quadrilateral.h"
#include "geometry/element/Tetrahedron.h"
#include "class/GroupViewTools.h"

namespace {
#ifdef _WIN32
    const std::string Separator = "\\";
#else
    const std::string Separator = "/";
#endif
}

namespace SEMBA {
namespace Exporters {
namespace VTK {

Exporter::Exporter(const UnstructuredProblemDescription& smb, const std::string& fn) :
    SEMBA::Exporters::Exporter(fn) 
{
    initDir_(fn + ".vtk");

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

void Exporter::writeMesh_(const UnstructuredProblemDescription& smb)
{
    const Geometry::Mesh::Unstructured* inMesh = &smb.model.mesh;
    const SourceGroup& srcs = smb.sources;
    const OutputRequestGroup& oRqs = smb.outputRequests;

    const Geometry::Grid3* grid = nullptr;

    assert(inMesh != nullptr);
    const Geometry::Mesh::Unstructured* mesh;
    const Geometry::Mesh::Structured* meshStr;

    std::string preName;
    meshStr = nullptr;
    mesh = inMesh;
    grid = nullptr;

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

    std::size_t part = 0;
    // Writes materials.
    if (smb.model.physicalModels.size() > 0) {
        for (auto const& lay : mesh->layers()) {
            for (auto const& mat : smb.model.physicalModels) {
                writeFile_(
                    mesh->elems().getMatLayerId(mat->getId(), lay->getId()),
                    makeValid_(preName + mat->getName() + "@" + lay->getName()),
                    outFile,
                    part
                );
            }
        }
    }
    else {
        for (auto const& lay : mesh->layers()) {
            writeFile_(
                mesh->elems().getLayerId(lay->getId()),
                makeValid_(preName + lay->getName()),
                outFile,
                part
            );
        }
    }
    // Writes EM Sources.
    for (const auto& source : srcs) {
        writeFile_(
            Class::Group::View::castToReal(source->getTarget()),
            makeValid_(preName + "EMSource_" + source->getName()),
            outFile,
            part
        );
    }

    // Writes output requests.
    for (const auto& oRq : oRqs) {
        writeFile_(
            Class::Group::View::castToReal(oRq->getTarget()),
            makeValid_(preName + "OutRq_" + oRq->getName()),
            outFile,
            part
        );
    }

    // Writes boundaries.
    if (meshStr != nullptr && grid != nullptr) {
        for (std::size_t i = 0; i < 3; i++) {
            for (std::size_t j = 0; j < 2; j++) {
                if (meshStr->bounds()(i, j) != nullptr) {
                    //Geometry::CoordR3Group cG;
                    writeFile_(
                        { getBoundary(
                        Math::Constants::CartesianAxis(i),
                        Math::Constants::CartesianBound(j),
                        Geometry::CoordR3Group(), grid, meshStr
                        ) },
                        makeValid_(getBoundaryName(meshStr, i, j)),
                        outFile,
                        part
                    );
                }
            }
        }
    }

    // Writes grid.
    if (grid != nullptr) {
        Geometry::CoordR3Group cG;
        writeFile_(getGridElems(cG, grid).get(), makeValid_("Grid"), outFile, part);
    }

    // Closes file.
    outFile << "  " << "</Collection>" << std::endl;
    outFile << "</VTKFile>" << std::endl;

    if (inMesh->is<Geometry::Mesh::Structured>()) {
        delete mesh;
    }

}
// En master, `const Group::Group<const Geometry::ElemR>& elems`
void Exporter::writeFile_(const ElemRView& elems,
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
        const ElemRView& elems) {
    std::map<Geometry::CoordId, std::size_t> mapCoords;
    std::vector<Math::CVecR3> pos;
    for (const auto& elem: elems) {
        for (std::size_t j = 0; j < elem->numberOfVertices(); j++) {
            if (mapCoords.count(elem->getVertex(j)->getId()) == 0) {
                mapCoords[elem->getVertex(j)->getId()] = pos.size();
                pos.push_back(elem->getVertex(j)->pos());
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
        const ElemRView& elems,
        std::map<Geometry::CoordId, std::size_t>& mapCoords) {

    outFile << "      " << "<Cells>" << std::endl;
    outFile << "        " << "<DataArray "
            << "type=\"Int32\" "
            << "Name=\"connectivity\" "
            << "format=\"ascii\""
            << ">" << std::endl;
    outFile << "         ";
    for (const auto& elem: elems) {
        for (std::size_t j = 0; j < elem->numberOfVertices(); j++) {
            outFile << " " << mapCoords.at(elem->getVertex(j)->getId());
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
    for (const auto& elem : elems) {
        offset += elem->numberOfVertices();
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
    for (const auto& elem : elems) {
        outFile << " ";
        if (elem->is<Geometry::Nod>()) {
            outFile << CELL_TYPES::VTK_VERTEX;
        } else if (elem->is<Geometry::Lin>()) {
            outFile << CELL_TYPES::VTK_LINE;
        } else if (elem->is<Geometry::Tri>()) {
            outFile << CELL_TYPES::VTK_TRIANGLE;
        } else if (elem->is<Geometry::Qua>()) {
            outFile << CELL_TYPES::VTK_QUAD;
        } else if (elem->is<Geometry::Tet>()) {
            outFile << CELL_TYPES::VTK_TETRA;
        } else if (elem->is<Geometry::Hex8>()) {
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
} /* namespace Exporters */
} /* namespace SEMBA */
