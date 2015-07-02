/*
 * ExporterVTK.cpp
 *
 *  Created on: 22/4/2015
 *      Author: Daniel
 */

#include <exporter/vtk/ExporterVTK.h>

namespace {
#ifdef _WIN32
    const string Separator = "\\";
#else
    const string Separator = "/";
#endif
}

ExporterVTK::ExporterVTK(const SmbData* smb,
        const string& fn)
:   Exporter(fn) {
    initDir_(fn);
    writeMesh_(smb);
}

ExporterVTK::~ExporterVTK() {

}

void ExporterVTK::writeMesh_(const SmbData* smb) {
    const Mesh* inMesh = smb->mesh;
    const GroupPhysicalModels<>* mat = smb->pMGroup;
    const GroupEMSources<>* srcs = smb->emSources;
    const GroupOutRqs<>* oRqs = smb->outputRequests;
    const Grid3* grid = NULL;
    assert(inMesh != NULL);
    assert(mat != NULL);
    const MeshUnstructured* mesh;
    string preName;
    if (inMesh->is<MeshStructured>()) {
        mesh = inMesh->castTo<MeshStructured>()->getMeshUnstructured();
        preName = "str_";
        grid = inMesh->castTo<Grid3>();
    } else {
        mesh = inMesh->cloneTo<MeshUnstructured>();
        grid = smb->grid;
    }

    string filename = getFilename() + ".pvd";
    ofstream outFile(filename.c_str());
    outFile << "<?xml version=\"1.0\"?>" << endl;
    outFile << "<VTKFile "
            << "type=\"Collection\" "
            << "version=\"0.1\" "
            << "byte_order=\"LittleEndian\" "
            << "compressor=\"vtkZLibDataCompressor\""
            << ">" << endl;
    outFile << "  " << "<Collection>" << endl;

    const GroupLayers<Layer>& lay = mesh->layers();
    UInt part = 0;
    // Writes materials.
    if (mat->size() > 0) {
        for (UInt i = 0; i < lay.size(); i++) {
            const LayerId layId = lay(i)->getId();
            for (UInt j = 0; j < mat->size(); j++) {
                const MatId matId = (*mat)(j)->getId();
                const string name = preName + (*mat)(j)->getName() + "@" + lay(i)->getName();
                GroupElements<const ElemR> elem = mesh->elems().getMatLayerId(matId, layId);
                writeFile_(elem, makeValid_(name), outFile, part);
            }
        }
    } else {
        for (UInt i = 0; i < lay.size(); i++) {
            const LayerId layId = lay(i)->getId();
            GroupElements<const ElemR> elem = mesh->elems().getLayerId(layId);
            const string name = preName + lay(i)->getName();
            writeFile_(elem, makeValid_(name), outFile, part);
        }
    }
    // Writes EM Sources.
    if (srcs != NULL) {
        for (UInt i = 0; i < srcs->size(); i++) {
            const EMSourceBase* src =  (*srcs)(i);
            const string name = preName + "EMSource_" + src->getName();
            GroupElements<const ElemR> elem =
                    mesh->elems().getId(src->elems().getIds());
            writeFile_(elem, makeValid_(name), outFile, part);
        }
    }
    // Writes output requests.
    if (oRqs != NULL) {
        for (UInt i = 0; i < oRqs->size(); i++) {
            const OutRqBase* oRq = (*oRqs)(i);
            const string name = preName + "OutRq_" + oRq->getName();
            GroupElements<const ElemR> elem =
                    mesh->elems().getId(oRq->elems().getIds());
            writeFile_(elem, makeValid_(name), outFile, part);
        }
    }
    // Writes boundaries.
    const OptionsMesher* opts = smb->mesherOptions;
    if (opts != NULL) {
        for (UInt i = 0; i < 3; i++) {
            for (UInt j = 0; j < 2; j++) {
                CoordR3Group cG;
                GroupElements<ElemR> bound =
                        getBoundary(CartesianAxis(i), CartesianBound(j), cG,
                                grid, mesh, opts);
                string name = getBoundaryName(opts, i, j);
                writeFile_(bound, makeValid_(name), outFile, part);
            }
        }
    }
    // Writes grid.
    CoordR3Group cG;
    GroupElements<ElemR> gridAux = getGridElems(cG, grid);
    writeFile_(gridAux, makeValid_("Grid"), outFile, part);
    // Closes file.
    outFile << "  " << "</Collection>" << endl;
    outFile << "</VTKFile>" << endl;

    delete mesh;
}

void ExporterVTK::writeFile_(const GroupElements<const ElemR>& elems,
        const string& name,
        ofstream& outMain,
        UInt& part) {
    if (elems.empty()) {
        return;
    }
    outMain << "    " << "<DataSet "
            << "group=\"" << name << "\" "
            << "part=\"" << part++ << "\" "
            << "file=\""
            << getBasename() + ".vtk" + Separator + name + ".vtu" << "\" "
            << "/>" << endl;

    string filename = getFilename() + ".vtk" + Separator + name + ".vtu";
    ofstream outFile(filename.c_str());
    outFile << "<VTKFile "
            << "type=\"UnstructuredGrid\" "
            << "version=\"0.1\" "
            << "byte_order=\"LittleEndian\""
            << ">" << endl;
    pair<vector<CVecR3>, map<CoordinateId, UInt>> aux = getPoints_(elems);
    outFile << "  " << "<UnstructuredGrid "
            << "NumberOfPoints=\"" << aux.first.size() << "\" "
            << "NumberOfCells=\"" << elems.size() << "\" "
            << ">" << endl;
    writePoints_(outFile, aux.first);
    writeCells_(outFile, elems, aux.second);
    outFile << "  " << "</UnstructuredGrid>" << endl;
    outFile << "</VTKFile>" << endl;
    outFile.close();
}

pair<vector<CVecR3>, map<CoordinateId, UInt>> ExporterVTK::getPoints_(
        const GroupElements<const ElemR>& elems) {
    map<CoordinateId, UInt> mapCoords;
    vector<CVecR3> pos;
    for (UInt i = 0; i < elems.size(); i++) {
        for (UInt j = 0; j < elems(i)->numberOfVertices(); j++) {
            if (mapCoords.count(elems(i)->getVertex(j)->getId()) == 0) {
                mapCoords[elems(i)->getVertex(j)->getId()] = pos.size();
                pos.push_back(elems(i)->getVertex(j)->pos());
            }
        }
    }
    return make_pair(pos, mapCoords);
}

void ExporterVTK::writePoints_(ofstream &outFile,
        const vector<CVecR3>& pos) {
    outFile << "      " << "<Points>" << endl;
    outFile << "        " << "<DataArray "
            << "type=\"Float32\" "
            << "NumberOfComponents=\"3\" "
            << "format=\"ascii\""
            << ">" << endl;
    outFile << "         ";
    for (UInt i = 0; i < pos.size(); i++) {
        outFile << " " << pos[i](x) << " " << pos[i](y) << " " << pos[i](z);
    }
    outFile << endl;
    outFile << "        " << "</DataArray>" << endl;
    outFile << "      " << "</Points>" << endl;
}

void ExporterVTK::writeCells_(ofstream &outFile,
        const GroupElements<const ElemR>& elems,
        const map<CoordinateId, UInt>& mapCoords) {

    outFile << "      " << "<Cells>" << endl;
    outFile << "        " << "<DataArray "
            << "type=\"Int32\" "
            << "Name=\"connectivity\" "
            << "format=\"ascii\""
            << ">" << endl;
    outFile << "         ";
    for (UInt i = 0; i < elems.size(); i++) {
        for (UInt j = 0; j < elems(i)->numberOfVertices(); j++) {
            outFile << " " << mapCoords.at(elems(i)->getVertex(j)->getId());
        }
    }
    outFile << endl;
    outFile << "        " << "</DataArray>" << endl;

    outFile << "        " << "<DataArray "
            << "type=\"Int32\" "
            << "Name=\"offsets\" "
            << "format=\"ascii\""
            << ">" << endl;
    outFile << "         ";
    Int offset = 0;
    for (UInt i = 0; i < elems.size(); i++) {
        offset += elems(i)->numberOfVertices();
        outFile << " " << offset;
    }
    outFile << endl;
    outFile << "        " << "</DataArray>" << endl;

    outFile << "        " << "<DataArray "
            << "type=\"UInt8\" "
            << "Name=\"types\" "
            << "format=\"ascii\""
            << ">" << endl;
    outFile << "         ";
    for (UInt i = 0; i < elems.size(); i++) {
        outFile << " ";
        if (elems(i)->is<Nod>()) {
            outFile << CELL_TYPES::VTK_VERTEX;
        } else if (elems(i)->is<Lin>()) {
            outFile << CELL_TYPES::VTK_LINE;
        } else if (elems(i)->is<Tri>()) {
            outFile << CELL_TYPES::VTK_TRIANGLE;
        } else if (elems(i)->is<Qua>()) {
            outFile << CELL_TYPES::VTK_QUAD;
        } else if (elems(i)->is<Tet>()) {
            outFile << CELL_TYPES::VTK_TETRA;
        } else if (elems(i)->is<Hex8>()) {
            outFile << CELL_TYPES::VTK_HEXAHEDRON;
        } else {
            outFile << CELL_TYPES::VTK_POLY_LINE;
        }
    }
    outFile << endl;
    outFile << "        " << "</DataArray>" << endl;
    outFile << "      " << "</Cells>" << endl;
}

string ExporterVTK::makeValid_(const string& allocator) {
    string res = allocator;
    std::replace(res.begin(), res.end(), '/', '_');
    return res;
}
