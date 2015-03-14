/*
 * OutputGiDSmb.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: luis
 */

#include "OutputGiDSmb.h"

OutputGiDSmb::OutputGiDSmb(const SmbData* smb) :
        OutputGiD(smb->getFilename()) {
    smb_ = smb;
    writeMesh();
}

OutputGiDSmb::OutputGiDSmb(const SmbData* smb, const string& fn) :
        OutputGiD(fn) {
    assert(smb != NULL);
    smb_ = smb;
    writeMesh();
}

OutputGiDSmb::~OutputGiDSmb() {
    GiD_ClosePostMeshFile();
}

void
OutputGiDSmb::writeMesh() {
    writeOutputRequestsMesh();
    LayerGroup<Layer> lay = smb_->mesh->layers().getGroupOf<Layer>();
    PhysicalModelGroup<PhysicalModel> mat =
        smb_->pMGroup->getGroupOf<PhysicalModel>();
    for (UInt i = 0; i < lay.size(); i++) {
        for (UInt j = 0; j < mat.size(); j++) {
            const MatId matId = mat(j)->getId();
            const LayerId layId = lay(i)->getId();
            const string name = mat(j)->getName() + "@" + lay(i)->getName();
            ElementsGroup<> elem = smb_->mesh->elems().get(matId, layId);
            writeElements(elem.getGroupOf<LinR2>().getGroupOf<ElementBase>(), name, GiD_Linear, 2);
            writeElements(elem.getGroupOf<Tri3>().getGroupOf<ElementBase>(), name, GiD_Triangle, 3);
            writeElements(elem.getGroupOf<Tet4>().getGroupOf<ElementBase>(), name, GiD_Tetrahedra, 4);
        }
    }
}

void
OutputGiDSmb::writeMeshWithIds(
        const vector<vector<ElementId> >& ids,
        const vector<string>& name) {
    assert(ids.size() == name.size());
    const bool isLinear = smb_->mesh->isLinear();
    Int nV;
    isLinear? nV = 4 : nV = 10;
    for (UInt t = 0; t < ids.size(); t++) {
        beginMesh(name[t], GiD_3D, GiD_Tetrahedra, nV);
        beginCoordinates();
        Int tmpCounter = coordCounter_;
        static const UInt GiDTetOrder[10] = {0, 4, 7, 9, 1, 5, 2, 3, 6, 8};
        for (UInt j = 0; j < ids[t].size(); j++) {
            const ElemR* e = smb_->mesh->elems().getPtrToId(ids[t][j])->castTo<ElemR>();
            for (Int i = 0; i < nV; i++) {
                CVecR3 pos;
                if (isLinear) {
                    pos = e->getVertex(i)->pos();
                } else {
                    pos = e->getV(GiDTetOrder[i])->pos();
                }
                writeCoordinates(++coordCounter_, pos);
            }
        }
        endCoordinates();
        // Writes elements.
        beginElements();
        int nId[nV];
        for (UInt j = 0; j < ids[t].size(); j++) {
            for (Int i = 0; i < nV; i++) {
                nId[i] = ++tmpCounter;
            }
            writeElement(++elemCounter_, nId);
        }
        endElements();
        endMesh();
    }
}

void
OutputGiDSmb::writeMeshWithIds(
        const vector<vector<ElementId> >& ids,
        string& name) {
    vector<string> names;
    for (UInt i = 0; i < ids.size(); i++) {
        stringstream ss;
        ss << name << " " << i;
        names.push_back(ss.str());
    }
    writeMeshWithIds(ids, names);
}

void
OutputGiDSmb::writeMeshWithIds(
        const vector<ElementId>& ids, string& name) {
    vector<vector<ElementId> > aux;
    aux.push_back(ids);
    writeMeshWithIds(aux, name);
}

void
OutputGiDSmb::writeOutputRequestsMesh() {
    for (UInt i = 0; i < smb_->outputRequests->size(); i++) {
        //        const OutputRequest* outRq = smb_->outputRequests->get(i);
        //      bool mshExist = false;
        //      for (UInt j = 0; j < result_.size(); j++) {
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

void OutputGiDSmb::writeElements(
        const ElementsGroup<>& elem,
        const string& name,
        const GiD_ElementType type,
        const Int nV) {
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
//OutputGiDSmb::writeMaterialsMesh() {
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
//OutputGiDSmb::writeBCMesh() {
//    BCGroup bc(smb_);
//    writeBCMesh(bc.get(Condition::pec), "PEC", pecColor);
//    writeBCMesh(bc.get(Condition::pmc), "PMC", pmcColor);
//    writeBCMesh(bc.get(Condition::sma), "SMA", smaColor);
//    writeBCMesh(bc.get(Condition::sibc), "SIBC", sibcColor);
//    writeBCMesh(bc.get(Condition::emSource), "EM Source", emSourceColor);
//}
//
//void
//OutputGiDSmb::writeBCMesh(
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
