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

OutputGiD::OutputGiD(const SmbData* smb) : Output(smb->getFilename()) {
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
    writeCurrentDensitySource();
    writeFieldSource();
    // Lines.
    for (uint i = 0; i < nfde->isotropicLine.size(); i++) {
        writeLine(&nfde->isotropicLine[i]);
    }
    for (uint i = 0; i < nfde->anisotropicLine.size(); i++) {
        writeLine(&nfde->anisotropicLine[i]);
    }
    // Surfs.
    for (uint i = 0; i < nfde->isotropicSurf.size(); i++) {
        writeSurf(&nfde->isotropicSurf[i]);
    }
    for (uint i = 0; i < nfde->anisotropicSurf.size(); i++) {
        writeSurf(&nfde->anisotropicSurf[i]);
    }
    for (uint i = 0; i < nfde->dispersiveSurf.size(); i++) {
        writeSurf(&nfde->dispersiveSurf[i]);
    }
    for (uint i = 0; i < nfde->compositeSurf.size(); i++) {
        writeSurf(&nfde->compositeSurf[i]);
    }
    // Bodies.
    for (uint i = 0; i < nfde->isotropicBody.size(); i++) {
        writeBody(&nfde->isotropicBody[i]);
    }
    for (uint i = 0; i < nfde->anisotropicBody.size(); i++) {
        writeBody(&nfde->anisotropicBody[i]);
    }
    for (uint i = 0; i < nfde->dispersiveBody.size(); i++) {
        writeBody(&nfde->dispersiveBody[i]);
    }

    writeThinWire();
    writeNewProbe();
    writeBulkProbes();
    writeSliceProbes();
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
            const Element* e = smb_->mesh->elem_.getPtrToId(ids[t][j]);
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
    vector<uint> vacuumIds = smb_->mesh->elem_.getIdsWithMaterialId(0);
    ids.push_back(smb_->mesh->getTetIds(vacuumIds));
    name.push_back("Vacuum");
    // Rest of materials.
    vector<uint> volMats = smb_->pMGroup->getVolumicMatIds();
    for (uint i = 0; i < volMats.size(); i++) {
        const uint matId = volMats[i];
        vector<uint> matIds = smb_->mesh->elem_.getIdsWithMaterialId(matId);
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
        const Element* e = smb_->mesh->elem_.getPtrToId(id);
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

void OutputGiD::writeCurrentDensitySource() {
    for(uint i = 0; i < nfde_->currentDensitySource.size(); i++) {
        const NFDEData::CurrentDensitySource& ent = nfde_->currentDensitySource[i];
        vector<const NFDEData::CoordsMultiplier*> cD;
        for (uint j = 0; j < ent.entities.size(); j++) {
            cD.push_back(&ent.entities[j]);
        }
        writeCoordMultiplier(cD, "Source//" + ent.getNameAtLayer());
    }
}

void OutputGiD::writeFieldSource() {
    for(uint i = 0; i < nfde_->fieldSource.size(); i++) {
        const NFDEData::FieldSource& ent = nfde_->fieldSource[i];
        vector<const NFDEData::CoordsMultiplier*> cD;
        for (uint j = 0; j < ent.entities.size(); j++) {
            cD.push_back(&ent.entities[j]);
        }
        writeCoordMultiplier(cD, "Source//" + ent.getNameAtLayer());
    }
}

void OutputGiD::writeLine(const NFDEData::Line* ent) {
    vector<const NFDEData::CoordsLine*> cD;
    for (uint j = 0; j < ent->entities.size(); j++) {
        cD.push_back(&ent->entities[j]);
    }
    writeCoordLines(cD, ent->getNameAtLayer());
}

void OutputGiD::writeSurf(const NFDEData::Surf* ent) {
    vector<const NFDEData::CoordsDir*> cD;
    for (uint j = 0; j < ent->entities.size(); j++) {
        cD.push_back(&ent->entities[j]);
    }
    writeCoordDirs(cD, ent->getNameAtLayer());
}

void OutputGiD::writeBody(const NFDEData::Body* ent) {
    vector<const NFDEData::Coords*> cD;
    for (uint j = 0; j < ent->entities.size(); j++) {
        cD.push_back(&ent->entities[j]);
    }
    writeCoords(cD, ent->getNameAtLayer());

}

void OutputGiD::writeThinWire() {
    for(uint i = 0; i < nfde_->thinWire.size(); i++) {
        const NFDEData::ThinWire& ent = nfde_->thinWire[i];
        vector<const NFDEData::CoordsLine*> wire, voltage, current, ext;
        for (uint j = 0; j < ent.segments.size(); j++) {
            if(abs(ent.segments[j].multiplier) <= 1e-4) {
                wire.push_back(&ent.segments[j]);
            } else {
                switch(ent.segments[j].srctype) {
                case NFDEData::CoordsWire::Types::VOLT:
                    voltage.push_back(&ent.segments[j]);
                    break;
                default:
                    current.push_back(&ent.segments[j]);
                }
            }
        }
        const string name = ent.getNameAtLayer();
        writeCoordLines(wire, name);
        writeCoordLines(current, name + "_Current_generator");
        writeCoordLines(voltage, name + "_Voltage_generator" );
        if (ent.tl != NFDEData::ThinWire::Extremes::NONE) {
            vector<const NFDEData::CoordsLine*> ext;
            ext.push_back(&ent.segments[0]);
            writeCoordLines(ext, name + "_TL_" + OutputNFDE::toString(ent.tl));
        }
        if (ent.tr != NFDEData::ThinWire::Extremes::NONE) {
            vector<const NFDEData::CoordsLine*> ext;
            ext.push_back(&ent.segments.back());
            writeCoordLines(ext, name + "_TR_" + OutputNFDE::toString(ent.tr));
        }
    }
}

void OutputGiD::writeNewProbe() {
    for (uint i = 0; i < nfde_->newProbe.size(); i++) {
        const NFDEData::NewProbe& out = nfde_->newProbe[i];
        vector<const NFDEData::CoordsNode*> node(out.entities.size());
        for (uint i = 0; i < out.entities.size(); i++) {
            node[i] = &out.entities[i];
        }
        writeCoordNodes(node, "OutRq//NewProbe_" + out.name);
    }
}

void OutputGiD::writeBulkProbes() {
    for (uint i = 0; i < nfde_->bulkProbe.size(); i++) {
        const NFDEData::BulkProbe& out = nfde_->bulkProbe[i];
        vector<const NFDEData::CoordsDir*> cD;
        cD.push_back(&out.entities);
        writeCoordDirs(cD, "OutRq//BulkProbe_" + out.name);
    }
}

void OutputGiD::writeSliceProbes() {
    for (uint i = 0; i < nfde_->sliceProbe.size(); i++) {
        const NFDEData::SliceProbe& out = nfde_->sliceProbe[i];
        vector<const NFDEData::Coords*> cD;
        cD.push_back(&out.entities);
        writeCoords(cD, "OutRq//SliceProbe_" + out.filename);
    }
}

void OutputGiD::writeTraditionalProbe() {
    for (uint i = 0; i < nfde_->traditionalProbe.size(); i++) {
        const NFDEData::TraditionalProbe& out = nfde_->traditionalProbe[i];
        vector<const NFDEData::Coords*> cD;
        cD.push_back(&out.entities);
        writeCoords(cD, "OutRq//TraditionalProbe_" + out.filename);
    }
}

void OutputGiD::writeCoordNodes(
        const vector<const NFDEData::CoordsNode*>& entities,
        const string& name) {
    static const int nV = 1;
    uint tmpCounter = coordCounter_;
    int nId[nV];
    beginMesh(name, GiD_3D, GiD_Point, nV);
    vector<CVecD3> pos;
    for(uint j = 0; j < entities.size(); j++) {
        CVecD3 aux;
        aux(x) = (double) entities[j]->coords(x);
        aux(y) = (double) entities[j]->coords(y);
        aux(z) = (double) entities[j]->coords(z);
        pos.push_back(aux);
    }
    writeCoordinates(pos);
    GiD_BeginElements();
    for (uint j = 0; j < entities.size(); j++) {
        for (int k = 0; k < nV; k++) {
            nId[k] = ++tmpCounter;
        }
        GiD_WriteElement(++elemCounter_, nId);

    }
    GiD_EndElements();
    GiD_EndMesh();
}

void OutputGiD::writeCoordMultiplier(
        const vector<const NFDEData::CoordsMultiplier*>& entities,
        const string& name) {
    vector<const NFDEData::CoordsLine*> lines(entities.size());
    for (uint i = 0; i < entities.size(); i++) {
           lines[i] = new NFDEData::CoordsLine(*entities[i]);
    }
    writeCoordLines(lines, name);
    lines.clear();
}


void OutputGiD::writeCoordLines(
        const vector<const NFDEData::CoordsLine*>& entities,
        const string& name) {
    static const int nV = 2;
    uint tmpCounter = coordCounter_;
    int nId[nV];
    beginMesh(name, GiD_3D, GiD_Linear, nV);
    vector<CVecD3> pos;
    for(uint j = 0; j < entities.size(); j++) {
        CVecD3 aux;
        aux(x) = (double) entities[j]->coords(x);
        aux(y) = (double) entities[j]->coords(y);
        aux(z) = (double) entities[j]->coords(z);
        pos.push_back(aux);
        aux(entities[j]->dir)++;
        pos.push_back(aux);
    }
    writeCoordinates(pos);
    GiD_BeginElements();
    for (uint j = 0; j < entities.size(); j++) {
        for (int k = 0; k < nV; k++) {
            nId[k] = ++tmpCounter;
        }
        GiD_WriteElement(++elemCounter_, nId);

    }
    GiD_EndElements();
    GiD_EndMesh();
}

void OutputGiD::writeCoordDirs(
        const vector<const NFDEData::CoordsDir*>& entities,
        const string& name) {
    static const uint nV = 4;
    double tmpCounter = coordCounter_;
    beginMesh(name, GiD_3D, GiD_Quadrilateral, nV);
    vector<CVecD3> pos;
    for(uint j = 0; j < entities.size(); j++) {
        vector<CVecD3> auxPos = BoxI3(entities[j]->coords).getPos();
        pos.insert(pos.end(), auxPos.begin(), auxPos.begin()+4);
    }
    writeCoordinates(pos);
    GiD_BeginElements();
    int nId[nV];
    for (uint i = 0; i < nV; i++) {
        nId[i] = ++tmpCounter;
    }
    GiD_WriteElement(++elemCounter_, nId);
    GiD_EndElements();
    GiD_EndMesh();
}

void OutputGiD::writeCoords(
        const vector<const NFDEData::Coords*>& entities,
        const string& name) {
    static const int nV = 8;
    uint tmpCounter = coordCounter_;
    beginMesh(name, GiD_3D, GiD_Hexahedra, nV);
    vector<CVecD3> pos;
    for(uint j = 0; j < entities.size(); j++) {
        vector<CVecD3> auxPos = BoxI3(entities[j]->coords).getPos();
        pos.insert(pos.end(), auxPos.begin(), auxPos.end());
    }
    writeCoordinates(pos);
    int nId[nV];
    GiD_BeginElements();
    for (uint j = 0; j < entities.size(); j++) {
        for (int k = 0; k < nV; k++) {
            nId[k] = ++tmpCounter;
        }
        GiD_WriteElement(++elemCounter_, nId);

    }
    GiD_EndElements();
    GiD_EndMesh();
}
