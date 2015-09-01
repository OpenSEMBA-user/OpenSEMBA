/*
 * GroupWires.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "GroupWires.h"

template<class T>
GroupWires<T>::GroupWires(const SmbData& smb) {
    init_(smb);
}

template<class T>
GroupWires<T>::~GroupWires() {
    for (UInt i = 0; i < wires_.size(); i++) {
        delete wires_[i];
    }
    for (map<MatId, PMWireExtremes*>::iterator
         it = mats_.begin(); it != mats_.end(); ++it) {
        delete it->second;
    }
}

template<class T>
void GroupWires<T>::printInfo() const {
    cout << "--- GroupWires Info ---" << endl;
    cout << "Wires:" << endl;
    for (UInt i = 0; i < wires_.size(); i++) {
        wires_[i]->printInfo();
    }
    cout << "Mats:" << endl;
    for (map<MatId, PMWireExtremes*>::const_iterator
         it = mats_.begin(); it != mats_.end(); ++it) {
        it->second->printInfo();
    }
}

template<class T>
void GroupWires<T>::init_(const SmbData& smb) {
    const Graph& graph = constructGraph_(smb);
    fillWiresInfo_(graph, smb);
}

template<class T>
typename GroupWires<T>::Graph
    GroupWires<T>::constructGraph_(const SmbData& smb) {

    GroupElements<const Line<T>> wires;
    GroupCoordinates<const Coordinate<T,3>> coords;
    const GroupPhysicalModels<>& mats = *smb.pMGroup;
    {
        GroupElements<const Line<T>> lines;
        if (is_floating_point<T>::value) {
            lines = smb.mesh->castTo<MeshUnstructured>()
                        ->elems().getOf<Line<T>>();
            coords = smb.mesh->castTo<MeshUnstructured>()
                         ->coords();
        } else if (is_integral<T>::value) {
            lines = smb.mesh->castTo<MeshStructured>()
                        ->elems().getOf<Line<T>>();
            coords = smb.mesh->castTo<MeshStructured>()
                         ->coords();
        }
        GroupPhysicalModels<const PMWire> pmwires =
            smb.pMGroup->getOf<PMWire>();
        vector<MatId> pmwiresIds = pmwires.getIds();
        GroupPhysicalModels<const PMMultiport> pmmults =
                mats.getOf<PMMultiport>();
        vector<MatId> pmmultsIds = pmmults.getIds();
        vector<MatId> matIds;
        matIds.reserve(pmwiresIds.size()+pmmultsIds.size());
        matIds.insert(matIds.end(), pmwiresIds.begin(), pmwiresIds.end());
        matIds.insert(matIds.end(), pmmultsIds.begin(), pmmultsIds.end());
        wires = lines.getMatId(matIds);
    }
    Graph graph;
    graph.init(wires, coords);
    const typename Graph::GraphBound* nodePtr;
    for(UInt i = 0; i < graph.numBounds(); i++) {
        nodePtr = graph.bound(i);
        if(nodePtr->numBounds() > 2) {
            graph.splitBound(i);
        } else if(nodePtr->numBounds() == 2) {
            LayerId layId[2];
            MatId matId[2];
            bool isWireMat[2];
            bool isWireExtrMat[2];
            bool extreme = true;
            for (UInt j = 0; j < 2; j++) {
                layId[j] = nodePtr->getBound(j)->elem()->getLayerId();
                matId[j] = nodePtr->getBound(j)->elem()->getMatId();
                isWireMat[j] = mats.getId(matId[j])->is<PMWire>();
                isWireExtrMat[j] = mats.getId(matId[j])->is<PMWireExtremes>();
                if (nodePtr->elem()->getId() !=
                    nodePtr->getBound(j)->getBound(0)->elem()->getId()) {
                    extreme = false;
                }
            }
            if (layId[0] != layId[1]) {
                graph.splitBound(i);
            } else if (isWireExtrMat[0] || isWireExtrMat[1]) {
                if ((!isWireExtrMat[0] || !isWireExtrMat[1]) ||
                    (matId[0] != matId[1])) {
                    graph.splitBound(i);
                }
            }
            else if (isWireMat[0] && isWireMat[1]) {
                if (matId[0] != matId[1]) {
                    graph.splitBound(i);
                }
            } else if (!isWireMat[0] && !isWireMat[1]) {
                if ((matId[0] != matId[1]) || extreme) {
                    graph.splitBound(i);
                }
            } else if (!isWireMat[0] &&
                       (nodePtr->elem()->getId() ==
                        nodePtr->getBound(0)->getBound(0)->elem()->getId())) {
                graph.splitBound(i);
            } else if (!isWireMat[1] &&
                       (nodePtr->elem()->getId() ==
                        nodePtr->getBound(1)->getBound(0)->elem()->getId())) {
                graph.splitBound(i);
            }
        }
    }
    return graph;
}

template<class T>
void GroupWires<T>::fillWiresInfo_(const GroupWires<T>::Graph& graph,
                                   const SmbData& smb) {
    vector<vector<const Line<T>*>> wires = getLines_(graph);
    const PMWire* wireMat;
    const PMMultiport *extremeL, *extremeR;
    for (UInt i = 0; i < wires.size(); i++) {
        wireMat = NULL;
        extremeL = extremeR = NULL;
        getWireMats_(wireMat, extremeL, extremeR, wires[i], smb);
        if (wireMat == NULL) {
            continue;
        }
        PMWireExtremes* wireExtremes;
        if (wireMat->is<PMWireExtremes>()) {
            wireExtremes = wireMat->cloneTo<PMWireExtremes>();
        } else {
            wireExtremes = new PMWireExtremes(*wireMat, extremeL, extremeR);
        }
        wireExtremes->setId(MatId(i+1));
        mats_[wireExtremes->getId()] = wireExtremes;
        Polyline<T>* newWire = newWire_(wires[i], wireExtremes->getId());
        if (newWire != NULL) {
            wires_.push_back(newWire);
        }
    }
}

template<class T>
vector<vector<const Line<T>*>> GroupWires<T>::getLines_(
        const GroupWires<T>::Graph& graph) {
    vector<vector<const Line<T>*>> res;
    set<ElementId> visLines;
    const typename Graph::GraphElem *prevSegment, *nextSegment;
    ElementId prvNodeId;
    for(UInt n = 1; n < 3; n++) {
        for(UInt i = 0; i < graph.numElems(); i++) {
            if ((graph.elem(i)->numNeighbors() == n) &&
               (visLines.count(graph.elem(i)->elem()->getId()) == 0)) {

                vector<const Line<T>*> wireLines;
                nextSegment = graph.elem(i);
                while(nextSegment != NULL) {
                    if(visLines.count(nextSegment->elem()->getId()) != 0) {
                        break;
                    }
                    wireLines.push_back(nextSegment->elem());
                    visLines.insert(nextSegment->elem()->getId());
                    prevSegment = nextSegment;
                    nextSegment = NULL;
                    for(UInt j = 0; j < prevSegment->numNeighbors(); j++) {
                        if (visLines.count(
                                prevSegment->getNeighbor(j)
                                    ->elem()->getId()) == 0) {
                            nextSegment = prevSegment->getNeighbor(j);
                        }
                    }
                    if(nextSegment == NULL) {
                        break;
                    }
                }
                if (!wireLines.empty()) {
                    res.push_back(wireLines);
                }
            }
        }
    }
    return res;
}

template<class T>
void GroupWires<T>::getWireMats_(const PMWire*& wireMat,
                                 const PMMultiport*& extremeL,
                                 const PMMultiport*& extremeR,
                                 const vector<const Line<T>*>& lines,
                                 const SmbData& smb) {

    const GroupPhysicalModels<>& mats = *smb.pMGroup;
    if (lines.empty()) {
        return;
    }
    vector<MatId> matIds;
    matIds.push_back(lines[0]->getMatId());
    for (UInt i = 1; i < lines.size(); i++) {
        if (lines[i]->getMatId() != matIds.back()) {
            matIds.push_back(lines[i]->getMatId());
        }
    }

    wireMat = NULL;
    extremeL = NULL;
    extremeR = NULL;
    for (UInt m = 0; m < matIds.size(); m++) {
        if (mats.getId(matIds[m])->is<PMMultiport>()) {
            if ((extremeL == NULL) &&
                (wireMat  == NULL)) {
                extremeL = mats.getId(matIds[m])->castTo<PMMultiport>();
            } else if ((extremeL != NULL) &&
                       (wireMat  == NULL)) {
                cout << "WARNING @ WireGroup: ";
                cout << " Wire: " << wires_.size()+1;
                cout << ". Two node materials together. ";
                cout << "Ignoring second.";
                cout << endl;
                assert(false);
                continue;
            } else if ((wireMat  != NULL) &&
                       (extremeR == NULL)) {
                extremeR = mats.getId(matIds[m])->castTo<PMMultiport>();
            } else if (extremeR != NULL) {
                cout << "WARNING @ WireGroup: ";
                cout << " Wire: " << wires_.size()+1;
                cout << ". Two node materials together. ";
                cout << "Ignoring second.";
                cout << endl;
                assert(false);
                continue;
            }
        } else if (mats.getId(matIds[m])->is<PMWire>()) {
            if (wireMat == NULL) {
                wireMat = mats.getId(matIds[m])->castTo<PMWire>();
            } else if (wireMat != NULL) {
                cout << "WARNING @ WireGroup: ";
                cout << " Wire: " << wires_.size()+1;
                cout << ". Too many wire materials. ";
                cout << "Ignoring last.";
                cout << endl;
                assert(false);
                continue;
            }
        }
    }
    if (wireMat == NULL) {
        cout << "WARNING @ WireGroup: ";
        cout << " Wire: " << wires_.size()+1;
        cout << ". No wire material. Ignoring wire.";
        cout << endl;
        assert(false);
        return;
    }
}

template<class T>
Polyline<T>* GroupWires<T>::newWire_(const vector<const Line<T>*>& lines,
                                     const MatId matId) {
    if (lines.empty()) {
        return NULL;
    }
    vector<const Coordinate<T,3>*> v;
    vector<ElementId> wireIds;
    wireIds.reserve(lines.size());
    const Coordinate<T,3>* prev = lines[0]->getV(0);
    if (lines.size() > 1) {
        if ((lines[0]->getV(0)->getId() == lines[1]->getV(0)->getId()) ||
            (lines[0]->getV(0)->getId() == lines[1]->getV(1)->getId())) {
            prev = lines[0]->getV(1);
        }
    }
    v.push_back(prev);
    for (UInt i = 0; i < lines.size(); i++) {
        wireIds.push_back(lines[i]->getId());
        if (prev->getId() != lines[i]->getV(0)->getId()) {
            prev = lines[i]->getV(0);
            v.push_back(prev);
        } else if (prev->getId() != lines[i]->getV(1)->getId()) {
            prev = lines[i]->getV(1);
            v.push_back(prev);
        }
    }
    LayerId layId = lines[0]->getLayerId();
    wiresIds_.push_back(wireIds);
    return new Polyline<T>(ElementId(wires_.size()+1), v, layId, matId);
}

template class GroupWires<Real>;
template class GroupWires<Int >;
