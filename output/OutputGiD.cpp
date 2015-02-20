/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#include "OutputGiD.h"

const CVecD3 OutputGiD::pecColor(255, 0, 0);
const CVecD3 OutputGiD::pmcColor(0, 255, 0);
const CVecD3 OutputGiD::smaColor(0, 0, 255);
const CVecD3 OutputGiD::pmlColor(0, 0, 255);
const CVecD3 OutputGiD::sibcColor(100, 0, 100);
const CVecD3 OutputGiD::emSourceColor(100, 100, 0);

OutputGiD::OutputGiD() {
    mode_ = GiD_PostAscii;
    coordCounter_ = 0;
    elemCounter_ = 0;
    smb_ = NULL;
    nfde_ = NULL;
}

OutputGiD::OutputGiD(
        const SmbData* smb) : Output(smb->getFilename()) {
    mode_ = GiD_PostAscii;
    coordCounter_ = 0;
    elemCounter_ = 0;
    smb_ = smb;
    nfde_ = NULL;
    openGiDFiles();
    writeMesh();
}

OutputGiD::OutputGiD(const NFDEData* nfde) : Output(nfde->getFilename()) {
    mode_ = GiD_PostAscii;
    coordCounter_ = 0;
    elemCounter_ = 0;
    smb_ = NULL;
    nfde_ = nfde;
    openGiDFiles();
    writeSpaceSteps();
    writeBoundaries();
    writePlaneWaveSource();
    //    writeCurrentDensitySource();
    //    writeFieldSource();
    writeIsotropicBody();
    //    writeIsotropicSurf();
    //    writeIsotropicLine();
    //    writeAnisotropicBody();
    //    writeAnisotropicSurf();
    //    writeAnisotropicLine();
    //    writeDispersiveBody();
    //    writeDispersiveSurf();
    //    writeCompositeSurf();
    //    writeThinWire();
    //    writeThinGap();
    //    writeTraditionalProbe();
    //    writeNewProbe();
    //    writeBulkProbes();
    //    writeSliceProbes();
    GiD_ClosePostMeshFile();
}

OutputGiD::~OutputGiD() {
    GiD_ClosePostResultFile();
}

void OutputGiD::beginMesh(
        const string& name,
        GiD_Dimension dim,
        GiD_ElementType elementType,
        int nNode,
        const CVecD3& colorRGB) const {
    char *tName;
    tName = new char[name.length() + 1];
    strcpy(tName, name.c_str());
    if (colorRGB == CVecD3(0.0, 0.0, 0.0)) {
        GiD_BeginMesh(tName, dim, elementType, nNode);
    } else {
        GiD_BeginMeshColor(tName, dim, elementType, nNode,
                (float) colorRGB(0), (float) colorRGB(1), (float) colorRGB(2));
    }
    delete [] tName;
}

void OutputGiD::beginResult(
        const string& fieldName,
        const string& timeName,
        const double time,
        GiD_ResultType resultType,
        GiD_ResultLocation resultLocaltion,
        const string gaussPointType,
        const vector<string>& componentsNames) const {
    char fName[fieldName.length() + 1];
    strcpy(fName, fieldName.c_str());
    char tName[timeName.length() + 1];
    strcpy(tName, timeName.c_str());
    char gpType[gaussPointType.length() + 1];
    strcpy(gpType, gaussPointType.c_str());
    vector<string> cNames = componentsNames;
    char *compv[cNames.size()];
    for (uint i = 0; i < cNames.size(); i++) {
        compv[i] = new char[cNames[i].length() + 1];
        strcpy(compv[i], cNames[i].c_str());
    }
    GiD_BeginResult(fName, tName, time, resultType,
            getGiDResultLocation(), gpType, NULL, cNames.size(), compv);
}

void OutputGiD::openPostMeshFile(
        const string& filename,
        const GiD_PostMode mode) const {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    GiD_OpenPostMeshFile(auxChar, mode);
    delete [] auxChar;
}

void OutputGiD::openPostResultFile(
        const string& filename,
        const GiD_PostMode mode) const {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    GiD_OpenPostResultFile(auxChar, mode);
    delete [] auxChar;
}

void
OutputGiD::flushPostFile() const {
    GiD_FlushPostFile();
}


void
OutputGiD::writeGaussPoints() const {
    //    {
    //        const char* name = "gp_tri";
    //        static const SimplexTri<ORDER_N> tri;
    //        GiD_BeginGaussPoint((char*) name, GiD_Triangle, NULL,tri.np,0,0);
    //        for (uint i = 0; i < tri.np; i++) {
    //            CVecD3 pos = tri.coordinate(i);
    //            GiD_WriteGaussPoint2D(pos(1), pos(2));
    //        }
    //        GiD_EndGaussPoint();
    //    }
    //    {
    //        const char* name = "gp_tet";
    //        static const SimplexTet<ORDER_N> tet;
    //        GiD_BeginGaussPoint((char*) name, GiD_Tetrahedra, NULL,tet.np,0,0);
    //        for (uint i = 0; i < tet.np; i++) {
    //            CartesianVector<double,4> pos = tet.coordinate(i);
    //            GiD_WriteGaussPoint3D(pos(1), pos(2), pos(3));
    //        }
    //        GiD_EndGaussPoint();
    //    }
}

GiD_ResultType OutputGiD::getGiDResultType(
        OutputRequest::Type type) const {
    switch (type) {
    case OutputRequest::electricField:
        return GiD_ResultType::GiD_Vector;
    case OutputRequest::magneticField:
        return GiD_ResultType::GiD_Vector;
    default:
        assert(false);
        return GiD_ResultType::GiD_Scalar;
    }
}

GiD_ResultLocation OutputGiD::getGiDResultLocation() const {
    return GiD_ResultLocation::GiD_OnNodes;
}


void OutputGiD::writeCoordinates(const uint id, const CVecD3 pos) const {
    GiD_WriteCoordinates(id, pos(x), pos(y), pos(z));
}

void OutputGiD::writeCoordinates(const vector<CVecD3>& pos)  {
    GiD_BeginCoordinates();
    for (uint i = 0; i < pos.size(); i++) {
        GiD_WriteCoordinates(++coordCounter_, pos[i](x), pos[i](y), pos[i](z));
    }
    GiD_EndCoordinates();
}

void OutputGiD::openGiDFiles() {
    deleteExistentOutputFiles();
    switch (mode_) {
    case GiD_PostAscii:
        openPostMeshFile(getOutputfilename() + ".post.msh", mode_);
//        openPostResultFile(getOutputfilename() + ".post.res", mode_);
        break;
    case GiD_PostBinary:
        openPostResultFile(getOutputfilename() + ".post.res", mode_);
        break;
    default:
        cerr << "ERROR @ GiDOutput::openFiles() " << endl;
    }
    writeGaussPoints();
}

//void
//OutputGiD::writeQuad4Mesh(
//        const string& name,
//        const CoordinateGroup& cG,
//        const vector<Quad4*> quad) {
//    static const int nV = 4;
//    beginMesh(name, GiD_3D, GiD_Quadrilateral, nV);
//    GiD_BeginCoordinates();
//    double coordCounterPreStart = coordCounter_;
////    for (uint i = 0; i <; i++) {
////        CVecD3 pos = mesh.v(i)->pos();
////        GiD_WriteCoordinates(++coordCounter_, pos(0), pos(1), pos(2));
////    }
//    GiD_EndCoordinates();
//    GiD_BeginElements();
//    int nId[nV + 1];
//    static const uint GiDOrder[4] = { 0, 1, 2, 3 };
//    for (uint e = 0; e < mesh.elem.quad4.size(); e++) {
//        const Quad4* quad = &mesh.elem.quad4[e];
//        for (int i = 0; i < nV; i++) {
//            nId[i] = quad->getVertex(GiDOrder[i])->getId() + 1
//                    + coordCounterPreStart;
//        }
//        nId[nV] = quad->getMatId();
//        GiD_WriteElementMat(++elemCounter_, nId);
//    }
//    GiD_EndElements();
//    GiD_EndMesh();
//}

void
OutputGiD::writeHex8Mesh(
        const string& name,
        const MeshVolume& mesh) {
    static const int nV = 8;
    beginMesh(name, GiD_3D, GiD_Hexahedra, nV);
    GiD_BeginCoordinates();
    double coordCounterPreStart = coordCounter_;
    CVecD3 pos;
    for (uint i = 0; i < mesh.v.size(); i++) {
        pos = *mesh.v(i);
        GiD_WriteCoordinates(++coordCounter_, pos(0), pos(1), pos(2));
    }
    GiD_EndCoordinates();
    GiD_BeginElements();
    int nId[nV + 1];
    static const uint GiDOrder[8] = { 0, 1, 2, 3, 4, 5, 6, 7};
    for (uint e = 0; e < mesh.elem.hex8.size(); e++) {
        const Hex8* hex = &mesh.elem.hex8[e];
        for (int i = 0; i < nV; i++) {
            nId[i] = hex->getVertex(GiDOrder[i])->getId() + 1
                    + coordCounterPreStart;
        }
        nId[nV] = hex->getMatId();
        GiD_WriteElementMat(++elemCounter_, nId);
    }
    GiD_EndElements();
    GiD_EndMesh();
}

void
OutputGiD::writeMesh() {
    writeOutputRequestsMesh();
    writeMainMesh();
    writeBCMesh();
    string name = "CurvedElements";
    writeMeshWithIds(smb_->mesh->getIdsOfCurvedTets(), name);
    if (mode_ == GiD_PostAscii) {
        GiD_ClosePostMeshFile();
    }
}

void
OutputGiD::writeMeshWithIds(
        const vector<vector<uint> >& ids,
        const vector<string>& name) {
    assert(ids.size() == name.size());
    const bool isLinear = smb_->mesh->isLinear();
    int nV;
    isLinear? nV = 4 : nV = 10;
    for (uint t = 0; t < ids.size(); t++) {
        beginMesh(name[t], GiD_3D, GiD_Tetrahedra, nV);
        GiD_BeginCoordinates();
        int tmpCounter = coordCounter_;
        static const uint GiDTetOrder[10] = {0, 4, 7, 9, 1, 5, 2, 3, 6, 8};
        for (uint j = 0; j < ids[t].size(); j++) {
            const Element* e = smb_->mesh->getElementWithId(ids[t][j]);
            for (int i = 0; i < nV; i++) {
                CVecD3 pos;
                if (isLinear) {
                    pos = e->getVertex(i)->pos();
                } else {
                    pos = e->getV(GiDTetOrder[i])->pos();
                }
                GiD_WriteCoordinates(++coordCounter_, pos(0), pos(1), pos(2));
            }
        }
        GiD_EndCoordinates();
        // Writes elements.
        GiD_BeginElements();
        int nId[nV];
        for (uint j = 0; j < ids[t].size(); j++) {
            for (int i = 0; i < nV; i++) {
                nId[i] = ++tmpCounter;
            }
            GiD_WriteElement(++elemCounter_, nId);
        }
        GiD_EndElements();
        GiD_EndMesh();
    }
}

void
OutputGiD::writeMeshWithIds(
        const vector<vector<uint> >& ids,
        string& name) {
    vector<string> names;
    for (uint i = 0; i < ids.size(); i++) {
        stringstream ss;
        ss << name << " " << i;
        names.push_back(ss.str());
    }
    writeMeshWithIds(ids, names);
}

void
OutputGiD::writeMeshWithIds(
        const vector<uint>& ids, string& name) {
    vector<vector<uint> > aux;
    aux.push_back(ids);
    writeMeshWithIds(aux, name);
}

void
OutputGiD::writeOutputRequestsMesh() {
    for (uint i = 0; i < smb_->outputRequests->count(); i++) {
//        const OutputRequest* outRq = smb_->outputRequests->get(i);
        //      bool mshExist = false;
        //      for (uint j = 0; j < result_.size(); j++) {
        //         mshExist = result_[i]->hasEquivalentMesh(outRq);
        //         if (mshExist) {
        //            result_.push_back(new ResultGiD(outRq, *result_[j]));
        //         }
        //      }
        //      if (!mshExist) {
        //        result_.push_back(
        //                new ResultGiD(outRq, coordCounter_, elemCounter_, dg_, smb_->mesh));
        //      }
    }
}

void
OutputGiD::writeMainMesh() {
    vector<vector<uint> > ids;
    vector<string> name;
    // Gets ids of tets with vacuum material.
    vector<uint> vacuumIds = smb_->mesh->getIdsWithMaterialId(0);
    ids.push_back(smb_->mesh->getTetIds(vacuumIds));
    name.push_back("Vacuum");
    // Rest of materials.
    vector<uint> volMats = smb_->pMGroup->getVolumicMatIds();
    for (uint i = 0; i < volMats.size(); i++) {
        const uint matId = volMats[i];
        vector<uint> matIds = smb_->mesh->getIdsWithMaterialId(matId);
        ids.push_back(smb_->mesh->getTetIds(matIds));
        name.push_back(smb_->pMGroup->getPMVolumeWithId(matId)->getName());
    }
    writeMeshWithIds(ids, name);
}

void
OutputGiD::writeBCMesh() {
    BCGroup bc(smb_);
    writeBCMesh(bc.get(Condition::pec), "PEC", pecColor);
    writeBCMesh(bc.get(Condition::pmc), "PMC", pmcColor);
    writeBCMesh(bc.get(Condition::sma), "SMA", smaColor);
    writeBCMesh(bc.get(Condition::sibc), "SIBC", sibcColor);
    writeBCMesh(bc.get(Condition::emSource), "EM Source", emSourceColor);
}

void
OutputGiD::writeBCMesh(
        const vector<const BoundaryCondition*>& bc,
        const string& nameIn,
        const CVecD3& RGB) {
    int nV;
    smb_->mesh->isLinear() ?  nV = 3 :  nV = 6;
    beginMesh(nameIn, GiD_3D, GiD_Triangle, nV, RGB);
    static const uint GiDOrder[6] = {0, 3, 5, 1, 4, 2};
    GiD_BeginCoordinates();
    uint tmpCounter = coordCounter_;
    for (uint j = 0; j < bc.size(); j++) {
        const uint id = bc[j]->getCell()->getId();
        const uint f = bc[j]->getFace();
        const Element* e = smb_->mesh->getElementWithId(id);
        for (int i = 0; i < nV; i++) {
            CVecD3 pos;
            if (smb_->mesh->isLinear()) {
                pos = e->getSideVertex(f,i)->pos();
            } else {
                pos = e->getSideV(f,GiDOrder[i])->pos();
            }
            writeCoordinates(++coordCounter_, pos);
        }
    }
    GiD_EndCoordinates();
    GiD_BeginElements();
    int nId[nV];
    for (uint i = 0; i < bc.size(); i++) {
        for (uint j = 0; j < (uint) nV; j++) {
            nId[j] = ++tmpCounter;
        }
        GiD_WriteElement(++elemCounter_, nId);
    }
    GiD_EndElements();
    GiD_EndMesh();
}

void OutputGiD::writeSpaceSteps() {
    beginMesh("Space_steps", GiD_3D, GiD_Linear, 2);
    GiD_BeginCoordinates();
    uint tmpCounter = coordCounter_;
    for (uint d = 0; d < 3; d++) {
        CVecD3 init;
        init(x) = nfde_->spacesteps[x].m;
        init(y) = nfde_->spacesteps[y].m;
        init(z) = nfde_->spacesteps[z].m;
        writeCoordinates(++coordCounter_, init);
        CVecD3 end(init);
        end(d) = nfde_->spacesteps[d].n;
        writeCoordinates(++coordCounter_, end);
    }
    GiD_EndCoordinates();
    GiD_BeginElements();
    int nId[2];
    for (uint d = 0; d < 3; d++) {
        for (uint j = 0; j < 2; j++) {
            nId[j] = ++tmpCounter;
        }
        GiD_WriteElement(++elemCounter_, nId);
    }
    GiD_EndElements();
    GiD_EndMesh();
}

string OutputGiD::makeValid(string name) {
    name.erase(remove(name.begin(), name.end(), '\n'), name.end());
    return name;
}

void OutputGiD::writeBoundaries() {
    BoxI3 box = nfde_->getFullDomainBox();
    for (uint d = 0; d < 3; d++) {
        for (uint p = 0; p < 2; p++) {
            vector<CVecD3> pos =
                    box.getPosOfBound(CartesianAxis(d), CartesianBound(p));
            string name = "Boundary//"
              + OutputNFDE::toString(nfde_->boundaries[d][p],d,p);
            beginMesh(makeValid(name), GiD_3D, GiD_Quadrilateral, 4);
            double tmpCounter = coordCounter_;
            writeCoordinates(pos);
            GiD_BeginElements();
            int nId[4];
            for (int i = 0; i < 4; i++) {
                nId[i] = ++tmpCounter;
            }
            GiD_WriteElement(++elemCounter_, nId);
            GiD_EndElements();
            GiD_EndMesh();
        }
    }
}

void OutputGiD::writePlaneWaveSource() {
    if (nfde_->planeWaveSource.size() == 0) {
        return;
    }
    static const int nV = 8;
    uint tmpCounter = coordCounter_;
    vector<CVecD3> pos=BoxI3(nfde_->planeWaveSource[0].coords.coords).getPos();
    beginMesh("Source//Planewave",GiD_3D, GiD_Hexahedra, nV, emSourceColor);
    writeCoordinates(pos);
    int nId[nV];
    for (int i = 0; i < nV; i++) {
        nId[i] = ++tmpCounter;
    }
    GiD_BeginElements();
    GiD_WriteElement(++elemCounter_, nId);
    GiD_EndElements();
    GiD_EndMesh();
}

void OutputGiD::writeDensitySource() {
}

void OutputGiD::writeFieldSource() {
}

void OutputGiD::writeIsotropicBody() {
    static const int nV = 8;
    uint tmpCounter = coordCounter_;
    int nId[nV];
    for(uint i = 0; i < nfde_->isotropicBody.size(); i++) {
        const NFDEData::IsotropicBody* ent = &nfde_->isotropicBody[i];
        const string name = ent->getNameAtLayer();
        beginMesh(name, GiD_3D, GiD_Hexahedra, nV);
        vector<CVecD3> pos;
        for(uint j = 0; j < ent->entities.size(); j++) {
            vector<CVecD3> auxPos = BoxI3(ent->entities[j].coords).getPos();
            pos.insert(pos.end(), auxPos.begin(), auxPos.end());
        }
        writeCoordinates(pos);
        GiD_BeginElements();
        for (uint j = 0; j < ent->entities.size(); j++) {
            for (int k = 0; k < nV; k++) {
                nId[k] = ++tmpCounter;
            }
            GiD_WriteElement(++elemCounter_, nId);

        }
        GiD_EndElements();
        GiD_EndMesh();
    }
}

void OutputGiD::writeIsotropicSurf() {
}

void OutputGiD::writeIsotropicLine() {
}

void OutputGiD::writeAnisotropicBody() {
}

void OutputGiD::writeAnisotropicSurf() {
}

void OutputGiD::writeAnisotropicLine() {
}

void OutputGiD::writeDispersiveBody() {
}

void OutputGiD::writeDispersiveSurf() {
}

void OutputGiD::writeCompositeSurf() {
}

void OutputGiD::writeThinWire() {
}

void OutputGiD::writeThinGap() {
}

void OutputGiD::writeTraditionalProbe() {
}

void OutputGiD::writeNewProbe() {
}

void OutputGiD::writeBulkProbes() {
}

void OutputGiD::writeSliceProbes() {
}
