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
    vector<uint> layId = smb_->layers->getIds();
    vector<uint> matId = smb_->pMGroup->getIds();
    for (uint i = 0; i < layId.size(); i++) {
        for (uint j = 0; j < matId.size(); j++) {
            const Layer* layer = smb_->layers->get(layId[i]);
            const PhysicalModel* mat = smb_->pMGroup->getPtrToId(matId[j]);
            const string name = mat->getName() + "@" + layer->getName();
            vector<const Element*> elem;
            elem = smb_->mesh->elem_.get(Element::line, matId[j], layId[i]);
            writeElements(elem, name, GiD_Linear, 2);
            elem = smb_->mesh->elem_.get(Element::surface, matId[j], layId[i]);
            writeElements(elem, name, GiD_Triangle, 3);
            elem = smb_->mesh->elem_.get(Element::volume, matId[j], layId[i]);
            writeElements(elem, name, GiD_Tetrahedra, 4);
        }
    }
}

void
OutputGiDSmb::writeMeshWithIds(
        const vector<vector<uint> >& ids,
        const vector<string>& name) {
    assert(ids.size() == name.size());
    const bool isLinear = smb_->mesh->isLinear();
    int nV;
    isLinear? nV = 4 : nV = 10;
    for (uint t = 0; t < ids.size(); t++) {
        beginMesh(name[t], GiD_3D, GiD_Tetrahedra, nV);
        beginCoordinates();
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
                writeCoordinates(++coordCounter_, pos);
            }
        }
        endCoordinates();
        // Writes elements.
        beginElements();
        int nId[nV];
        for (uint j = 0; j < ids[t].size(); j++) {
            for (int i = 0; i < nV; i++) {
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
OutputGiDSmb::writeMeshWithIds(
        const vector<uint>& ids, string& name) {
    vector<vector<uint> > aux;
    aux.push_back(ids);
    writeMeshWithIds(aux, name);
}

void
OutputGiDSmb::writeOutputRequestsMesh() {
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

void OutputGiDSmb::writeElements(
        const vector<const Element*>& elem,
        const string& name,
        const GiD_ElementType type,
        const int nV) {
    uint tmpCounter = coordCounter_;
    int nId[nV];
    beginMesh(name, GiD_3D, type, nV);
    vector<CVecD3> pos;
    for(uint i = 0; i < elem.size(); i++) {
        for (uint j = 0; j < nV; j++) {
            pos.push_back(elem[i]->getVertex(j)->pos());
        }
    }
    writeCoordinates(pos);
    beginElements();
    for (uint j = 0; j < elem.size(); j++) {
        for (int k = 0; k < nV; k++) {
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
//    vector<vector<uint> > ids;
//    vector<string> name;
//    // Gets ids of tets with vacuum material.
//    vector<uint> vacuumIds = smb_->mesh->elem_.getIdsWithMaterialId(0);
//    ids.push_back(smb_->mesh->getTetIds(vacuumIds));
//    name.push_back("Vacuum");
//    // Rest of materials.
//    vector<uint> volMats = smb_->pMGroup->getVolumicMatIds();
//    for (uint i = 0; i < volMats.size(); i++) {
//        const uint matId = volMats[i];
//        vector<uint> matIds = smb_->mesh->elem_.getIdsWithMaterialId(matId);
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
//        const CVecD3& RGB) {
//    int nV;
//    smb_->mesh->isLinear() ?  nV = 3 :  nV = 6;
//    beginMesh(nameIn, GiD_3D, GiD_Triangle, nV, RGB);
//    static const uint GiDOrder[6] = {0, 3, 5, 1, 4, 2};
//    beginCoordinates();
//    uint tmpCounter = coordCounter_;
//    for (uint j = 0; j < bc.size(); j++) {
//        const uint id = bc[j]->getCell()->getId();
//        const uint f = bc[j]->getFace();
//        const Element* e = smb_->mesh->elem_.getPtrToId(id);
//        for (int i = 0; i < nV; i++) {
//            CVecD3 pos;
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
//    int nId[nV];
//    for (uint i = 0; i < bc.size(); i++) {
//        for (uint j = 0; j < (uint) nV; j++) {
//            nId[j] = ++tmpCounter;
//        }
//        writeElement(++elemCounter_, nId);
//    }
//    endElements();
//    endMesh();
//}
