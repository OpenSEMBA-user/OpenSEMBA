/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#include "OutputGiD.h"

Int OutputGiD::numberOfOutputGiD_ = 0;
Int OutputGiD::coordCounter_ = 0;
Int OutputGiD::elemCounter_ = 0;

const CVecR3 OutputGiD::pecColor(255, 0, 0);
const CVecR3 OutputGiD::pmcColor(0, 255, 0);
const CVecR3 OutputGiD::smaColor(0, 0, 255);
const CVecR3 OutputGiD::pmlColor(0, 0, 255);
const CVecR3 OutputGiD::sibcColor(100, 0, 100);
const CVecR3 OutputGiD::emSourceColor(100, 100, 0);

void OutputGiD::initDefault(
        GiD_PostMode mode,
        const string& fn) {
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
        openPostMeshFile(fn + ".post.msh");
        break;
    case GiD_PostBinary:
        openPostResultFile(fn + ".post.res");
        break;
    default:
        cerr << endl << "ERROR @ GiDOutput::openFiles() " << endl;
    }
    writeGaussPoints();
}

OutputGiD::OutputGiD(
        const SmbData* smb,
        const string& fn,
        GiD_PostMode mode) : Output(fn) {
    initDefault(mode, fn);
    writeMesh(smb->mesh, smb->pMGroup, smb->emSources, smb->outputRequests);
}

OutputGiD::OutputGiD(
        const MeshUnstructured* mesh,
        const PhysicalModelGroup<>* mat,
        const string& fn, GiD_PostMode mode) : Output(fn) {
    initDefault(mode, fn);
    writeMesh(mesh, mat);
}

OutputGiD::~OutputGiD() {
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

void OutputGiD::writeAllElements(const ElemRGroup& elem, const string& name) {
    writeElements(elem.getGroupOf<NodeR>(),  name, GiD_Point, 1);
    writeElements(elem.getGroupOf<LinR2>(),  name, GiD_Linear, 2);
    writeElements(elem.getGroupOf<Tri3>(),   name, GiD_Triangle, 3);
    writeElements(elem.getGroupOf<QuadR4>(), name, GiD_Quadrilateral, 4);
    writeElements(elem.getGroupOf<Tet4>(),   name, GiD_Tetrahedra, 4);
    writeElements(elem.getGroupOf<HexR8>(),  name, GiD_Hexahedra, 8);
}

void
OutputGiD::writeMesh(
        const Mesh* inMesh,
        const PhysicalModelGroup<>* mat,
        const EMSourceGroup<>* srcs,
        const OutRqGroup<>* oRqs) {
    assert(inMesh != NULL);
    assert(mat != NULL);
    const MeshUnstructured* mesh;
    if (inMesh->is<MeshStructured>()) {
        mesh = inMesh->castTo<MeshStructured>()->getMeshUnstructured();
    } else {
        mesh = new MeshUnstructured(*inMesh->castTo<MeshUnstructured>());
    }
    // Writes materials.
    LayerGroup<> lay = mesh->layers();
    for (UInt i = 0; i < lay.size(); i++) {
        for (UInt j = 0; j < mat->size(); j++) {
            const MatId matId = (*mat)(j)->getId();
            const LayerId layId = lay(i)->getId();
            const string name = (*mat)(j)->getName() + "@" + lay(i)->getName();
            ElemRGroup elem = mesh->elems().get(matId, layId);
            writeAllElements(elem, name);
        }
    }
    // Writes EM Sources.
    if (srcs != NULL) {
        for (UInt i = 0; i < srcs->size(); i++) {
            const EMSource<>* src =  (*srcs)(i);
            const string name = "EMSource_" + src->getName();
            ElemRGroup elem = src->elems();
            writeAllElements(elem, name);
        }
    }
    // Writes output requests.
    if (oRqs != NULL) {
        for (UInt i = 0; i < oRqs->size(); i++) {
            const OutRq<>* oRq = (*oRqs)(i);
            const string name = "OutRq_" + oRq->getName();
            ElemRGroup elem = oRq->elems();
            writeAllElements(elem, name);
        }
    }
    //
    delete mesh;
}

//void
//OutputGiD::writeMeshWithIds(
//        const vector<vector<ElementId> >& ids,
//        const vector<string>& name) {
//    assert(ids.size() == name.size());
//    const bool isLinear = mesh_->isLinear();
//    Int nV;
//    isLinear? nV = 4 : nV = 10;
//    for (UInt t = 0; t < ids.size(); t++) {
//        beginMesh(name[t], GiD_3D, GiD_Tetrahedra, nV);
//        beginCoordinates();
//        Int tmpCounter = coordCounter_;
//        static const UInt GiDTetOrder[10] = {0, 4, 7, 9, 1, 5, 2, 3, 6, 8};
//        for (UInt j = 0; j < ids[t].size(); j++) {
//            const ElemR* e = mesh_->elems().getPtrToId(ids[t][j])->castTo<ElemR>();
//            for (Int i = 0; i < nV; i++) {
//                CVecR3 pos;
//                if (isLinear) {
//                    pos = e->getVertex(i)->pos();
//                } else {
//                    pos = e->getV(GiDTetOrder[i])->pos();
//                }
//                writeCoordinates(++coordCounter_, pos);
//            }
//        }
//        endCoordinates();
//        // Writes elements.
//        beginElements();
//        int nId[nV];
//        for (UInt j = 0; j < ids[t].size(); j++) {
//            for (Int i = 0; i < nV; i++) {
//                nId[i] = ++tmpCounter;
//            }
//            writeElement(++elemCounter_, nId);
//        }
//        endElements();
//        endMesh();
//    }
//}
//
//void
//OutputGiD::writeMeshWithIds(
//        const vector<vector<ElementId> >& ids,
//        string& name) {
//    vector<string> names;
//    for (UInt i = 0; i < ids.size(); i++) {
//        stringstream ss;
//        ss << name << " " << i;
//        names.push_back(ss.str());
//    }
//    writeMeshWithIds(ids, names);
//}
//
//void
//OutputGiD::writeMeshWithIds(
//        const vector<ElementId>& ids, string& name) {
//    vector<vector<ElementId> > aux;
//    aux.push_back(ids);
//    writeMeshWithIds(aux, name);
//}
//
//void
//OutputGiD::writeOutputRequestsMesh() {
//    for (UInt i = 0; i < smb_->outputRequests->size(); i++) {
//        //        const OutputRequest* outRq = smb_->outputRequests->get(i);
//        //      bool mshExist = false;
//        //      for (UInt j = 0; j < result_.size(); j++) {
//        //         mshExist = result_[i]->hasEquivalentMesh(outRq);
//        //         if (mshExist) {
//        //            result_.push_back(new ResultGiD(outRq, *result_[j]));
//        //         }
//        //      }
//        //      if (!mshExist) {
//        //        result_.push_back(
//        //                new ResultGiD(outRq, coordCounter_, elemCounter_, dg_, smb_->mesh));
//        //      }
//    }
//}

void OutputGiD::writeElements(
        const ElemRGroup& elem,
        const string& name,
        const GiD_ElementType type,
        const Int nV) {
    if (elem.size() == 0) {
        return;
    }
    UInt tmpCounter = coordCounter_;
    int nId[nV];
    beginMesh(name, GiD_3D, type, nV);
    vector<CVecR3> pos;
    for(UInt i = 0; i < elem.size(); i++) {
        for (Int j = 0; j < nV; j++) {
            pos.push_back(elem(i)->getVertex(j)->pos());
        }
    }
    writeCoordinates(pos);
    beginElements();
    for (UInt j = 0; j < elem.size(); j++) {
        for (Int k = 0; k < nV; k++) {
            nId[k] = ++tmpCounter;
        }
        writeElement(++elemCounter_, nId);

    }
    endElements();
    endMesh();
}

//
//void
//OutputGiD::writeMaterialsMesh() {
//    vector<vector<UInt> > ids;
//    vector<string> name;
//    // Gets ids of tets with vacuum material.
//    vector<UInt> vacuumIds = smb_->mesh->elem_.getIdsWithMaterialId(0);
//    ids.push_back(smb_->mesh->getTetIds(vacuumIds));
//    name.push_back("Vacuum");
//    // Rest of materials.
//    vector<UInt> volMats = smb_->pMGroup->getVolumicMatIds();
//    for (UInt i = 0; i < volMats.size(); i++) {
//        const UInt matId = volMats[i];
//        vector<UInt> matIds = smb_->mesh->elem_.getIdsWithMaterialId(matId);
//        ids.push_back(smb_->mesh->getTetIds(matIds));
//        name.push_back(smb_->pMGroup->getPMVolumeWithId(matId)->getName());
//    }
//    writeMeshWithIds(ids, name);
//}

//void
//OutputGiD::writeBCMesh() {
//    BCGroup bc(smb_);
//    writeBCMesh(bc.get(Condition::pec), "PEC", pecColor);
//    writeBCMesh(bc.get(Condition::pmc), "PMC", pmcColor);
//    writeBCMesh(bc.get(Condition::sma), "SMA", smaColor);
//    writeBCMesh(bc.get(Condition::sibc), "SIBC", sibcColor);
//    writeBCMesh(bc.get(Condition::emSource), "EM Source", emSourceColor);
//}
//
//void
//OutputGiD::writeBCMesh(
//        const vector<const BoundaryCondition*>& bc,
//        const string& nameIn,
//        const CVecR3& RGB) {
//    Int nV;
//    smb_->mesh->isLinear() ?  nV = 3 :  nV = 6;
//    beginMesh(nameIn, GiD_3D, GiD_Triangle, nV, RGB);
//    static const UInt GiDOrder[6] = {0, 3, 5, 1, 4, 2};
//    beginCoordinates();
//    UInt tmpCounter = coordCounter_;
//    for (UInt j = 0; j < bc.size(); j++) {
//        const UInt id = bc[j]->getCell()->getId();
//        const UInt f = bc[j]->getFace();
//        const Element* e = smb_->mesh->elem_.getPtrToId(id);
//        for (Int i = 0; i < nV; i++) {
//            CVecR3 pos;
//            if (smb_->mesh->isLinear()) {
//                pos = e->getSideVertex(f,i)->pos();
//            } else {
//                pos = e->getSideV(f,GiDOrder[i])->pos();
//            }
//            writeCoordinates(++coordCounter_, pos);
//        }
//    }
//    endCoordinates();
//    beginElements();
//    Int nId[nV];
//    for (UInt i = 0; i < bc.size(); i++) {
//        for (UInt j = 0; j < (UInt) nV; j++) {
//            nId[j] = ++tmpCounter;
//        }
//        writeElement(++elemCounter_, nId);
//    }
//    endElements();
//    endMesh();
//}


void OutputGiD::beginMesh(
        const string& name,
        GiD_Dimension dim,
        GiD_ElementType elementType,
        Int nNode,
        const CVecR3& colorRGB) const {
    char *tName;
    tName = new char[name.length() + 1];
    strcpy(tName, name.c_str());
    if (colorRGB == CVecR3(0.0, 0.0, 0.0)) {
        GiD_fBeginMesh(meshFile_, tName, dim, elementType, nNode);
    } else {
        GiD_fBeginMeshColor(meshFile_, tName, dim, elementType, nNode,
                (Real) colorRGB(0), (Real) colorRGB(1), (Real) colorRGB(2));
    }
    delete [] tName;
}

void OutputGiD::beginResult(
        const string& fieldName,
        const string& timeName,
        const Real time,
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
    const char *compv[cNames.size()];
    for (UInt i = 0; i < cNames.size(); i++) {
        compv[i] = cNames[i].c_str();
//        compv[i] = new char[cNames[i].length() + 1];
//        strcpy(compv[i], cNames[i].c_str());
    }
    GiD_fBeginResult(resultFile_, fName, tName, time, resultType,
            getGiDResultLocation(), gpType, NULL, cNames.size(), compv);
}

void OutputGiD::openPostMeshFile(
        const string& filename) {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    meshFile_ = GiD_fOpenPostMeshFile(auxChar, mode_);
    delete [] auxChar;
}

void OutputGiD::openPostResultFile(
        const string& filename)  {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    resultFile_ = GiD_fOpenPostResultFile(auxChar, mode_);
    if (mode_ == GiD_PostBinary) {
        meshFile_ = resultFile_;
    }
    delete [] auxChar;
}

void
OutputGiD::flushPostFile() const {
    if (meshFile_ != 0) {
        GiD_fFlushPostFile(meshFile_);
    }
    if (resultFile_ != 0) {
        GiD_fFlushPostFile(resultFile_);
    }
}


void
OutputGiD::writeGaussPoints() const {
    //    {
    //        const char* name = "gp_tri";
    //        static const SimplexTri<ORDER_N> tri;
    //        GiD_BeginGaussPoint((char*) name, GiD_Triangle, NULL,tri.np,0,0);
    //        for (UInt i = 0; i < tri.np; i++) {
    //            CVecR3 pos = tri.coordinate(i);
    //            GiD_WriteGaussPoint2D(pos(1), pos(2));
    //        }
    //        GiD_EndGaussPoint();
    //    }
    //    {
    //        const char* name = "gp_tet";
    //        static const SimplexTet<ORDER_N> tet;
    //        GiD_BeginGaussPoint((char*) name, GiD_Tetrahedra, NULL,tet.np,0,0);
    //        for (UInt i = 0; i < tet.np; i++) {
    //            CartesianVector<Real,4> pos = tet.coordinate(i);
    //            GiD_WriteGaussPoint3D(pos(1), pos(2), pos(3));
    //        }
    //        GiD_EndGaussPoint();
    //    }
}

GiD_ResultType OutputGiD::getGiDResultType(OutRq<>::Type type) const {
    switch (type) {
    case OutRq<>::electric:
        return GiD_ResultType::GiD_Vector;
    case OutRq<>::magnetic:
        return GiD_ResultType::GiD_Vector;
    default:
        assert(false);
        return GiD_ResultType::GiD_Scalar;
    }
}

GiD_ResultLocation OutputGiD::getGiDResultLocation() const {
    return GiD_ResultLocation::GiD_OnNodes;
}


void OutputGiD::writeCoordinates(const UInt id, const CVecR3 pos) const {
    GiD_fWriteCoordinates(meshFile_, id, pos(x), pos(y), pos(z));
}

void OutputGiD::writeCoordinates(const vector<CVecR3>& pos)  {
    GiD_fBeginCoordinates(meshFile_);
    for (UInt i = 0; i < pos.size(); i++) {
        writeCoordinates(++coordCounter_, pos[i]);
    }
    GiD_fEndCoordinates(meshFile_);
}

void OutputGiD::writeElement(Int elemId, int nId[]) const {
    GiD_fWriteElement(meshFile_, elemId, nId);
}

string OutputGiD::makeValid(string name) {
    name.erase(remove(name.begin(), name.end(), '\n'), name.end());
    return name;
}

void OutputGiD::beginCoordinates() const {
    GiD_fBeginCoordinates(meshFile_);
}

void OutputGiD::endCoordinates() const {
    GiD_fEndCoordinates(meshFile_);
}

void OutputGiD::beginElements() const {
    GiD_fBeginElements(meshFile_);
}

void OutputGiD::endElements() const {
    GiD_fEndElements(meshFile_);
}

void OutputGiD::endMesh() const {
    GiD_fEndMesh(meshFile_);
}
