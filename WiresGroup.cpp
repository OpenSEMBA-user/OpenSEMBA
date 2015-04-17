/*
 * WireGroup.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "WiresGroup.h"

template<class T>
WiresGroup<T>::WiresGroup(const SmbData& smb) {
    init_(smb);
}

template<class T>
WiresGroup<T>::~WiresGroup() {
    for (UInt i = 0; i < wires_.size(); i++) {
        delete wires_[i];
    }
    for (map<MatId, PMWireExtremes*>::iterator
         it = mats_.begin(); it != mats_.end(); ++it) {
        delete it->second;
    }
}

template<class T>
void WiresGroup<T>::init_(const SmbData& smb) {
    const GraphLines<T>& graph = constructGraph_(smb);
    fillWiresInfo_(graph, smb);
}

template<class T>
GraphLines<T> WiresGroup<T>::constructGraph_(const SmbData& smb) {
    ElementsGroup<const Line<T>> wires;
    const PhysicalModelGroup<>& mats = *smb.pMGroup;
    {
        ElementsGroup<const Line<T>> lines;
        if (is_floating_point<T>::value) {
            lines = smb.mesh->castTo<MeshStructured>()
                        ->elems().getGroupOf<Line<T>>();
        } else if (is_integral<T>::value) {
            lines = smb.mesh->castTo<MeshUnstructured>()
                        ->elems().getGroupOf<Line<T>>();
        }
        PhysicalModelGroup<const PMWire> pmwires =
            smb.pMGroup->getGroupOf<PMWire>();
        wires.add(lines.getGroupWith(pmwires.getIds()));

        PhysicalModelGroup<const PMMultiport> pmmults =
                mats.getGroupOf<PMMultiport>();
        wires.add(lines.getGroupWith(pmmults.getIds()));
    }

    GraphLines<T> graph(wires);

    const VertexCoord<T>* nodePtr;

    for(UInt i = 0; i < graph.numCoords(); i++) {
        nodePtr = graph.coord(i);

        if(nodePtr->numLines() > 2) {
            graph.splitNode(i);
        } else if(nodePtr->numLines() == 2) {
            LayerId layId[2];
            MatId matId[2];
            bool isWireMat[2];
            bool isWireExtrMat[2];
            bool extreme = true;
            for (UInt j = 0; j < 2; j++) {
                layId[j] = nodePtr->getLine(j)->line()->getLayerId();
                matId[j] = nodePtr->getLine(j)->line()->getMatId();
                isWireMat[j] = mats.get(matId[j])->is<PMWire>();
                isWireExtrMat[j] = mats.get(matId[j])->is<PMWireExtremes>();
                if (nodePtr->coord()->getId() !=
                    nodePtr->getLine(j)->getCoord(0)->coord()->getId()) {
                    extreme = false;
                }
            }
            if (layId[0] != layId[1]) {
                graph.splitNode(i);
            } else if (isWireExtrMat[0] || isWireExtrMat[1]) {
                if ((!isWireExtrMat[0] || !isWireExtrMat[1]) ||
                    (matId[0] != matId[1])) {
                    graph.splitNode(i);
                }
            }
            else if (isWireMat[0] && isWireMat[1]) {
                if (matId[0] != matId[1]) {
                    graph.splitNode(i);
                }
            } else if (!isWireMat[0] && !isWireMat[1]) {
                if ((matId[0] != matId[1]) || extreme) {
                    graph.splitNode(i);
                }
            } else if (!isWireMat[0] &&
                       (nodePtr->coord()->getId() ==
                        nodePtr->getLine(0)->getCoord(0)->coord()->getId())) {
                graph.splitNode(i);
            } else if (!isWireMat[1] &&
                       (nodePtr->coord()->getId() ==
                        nodePtr->getLine(1)->getCoord(0)->coord()->getId())) {
                graph.splitNode(i);
            }
        }
    }

    return graph;
}

template<class T>
void WiresGroup<T>::fillWiresInfo_(const GraphLines<T>& graph,
                                   const SmbData& smb) {

    const PhysicalModelGroup<>& mats = *smb.pMGroup;
    vector<vector<const Line<T>*>> wires;
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
vector<vector<const Line<T>*>> WiresGroup<T>::getLines_(
        const GraphLines<T>& graph) {

    vector<vector<const Line<T>*>> res;
    set<const VertexLine<T>*> visLines;
    const VertexLine<T> *prevSegment, *nextSegment;
    ElementId prvNodeId;
    for(unsigned int n = 1; n < 3; n++) {
        for(unsigned int i = 0; i < graph.numCoords(); i++) {
            if ((graph.coord(i)->numLines() == n) &&
                (visLines.count(graph.coord(i)->getLine(0)) == 0)) {

                vector<const Line<T>*> wireLines;
                nextSegment = graph.coord(i)->getLine(0);
                while(nextSegment != NULL) {
                    if(visLines.count(nextSegment) != 0) {
                        break;
                    }
                    wireLines.push_back(nextSegment->line());
                    visLines.insert(nextSegment);
                    prevSegment = nextSegment;
                    nextSegment = NULL;
                    for(UInt j = 0; j < prevSegment->numLines(); j++) {
                        if(visLines.count(prevSegment->getLine(j)) == 0) {
                            nextSegment = prevSegment->getLine(j);
                        }
                    }
                    if(nextSegment == NULL) {
                        break;
                    }
                }
                if (wireLines.empty()) {
                    continue;
                }
                res.push_back(wireLines);
            }
        }
    }
    return res;
}

template<class T>
void WiresGroup<T>::getWireMats_(const PMWire* wireMat,
                                 const PMMultiport* extremeL,
                                 const PMMultiport* extremeR,
                                 const vector<const Line<T> *>& lines,
                                 const SmbData& smb) {

    const PhysicalModelGroup<>& mats = *smb.pMGroup;
    wireMat = NULL;
    extremeL = extremeR = NULL;
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

    Int extremeLPos = -1;
    Int wireMatPos  = -1;
    Int extremeRPos = -1;
    for (UInt m = 0; m < mats.size(); m++) {
        if (mats.get(matIds[m])->is<PMMultiport>()) {
            if ((extremeLPos == -1) &&
                (wireMatPos  == -1)) {
                extremeLPos = m;
            } else if ((extremeLPos >  -1) &&
                       (wireMatPos  == -1)) {
                cerr << "WARNING @ WireGroup: ";
                cerr << " Wire: " << wires_.size()+1;
                cerr << ". Two node materials together. ";
                cerr << "Ignoring second.";
                cerr << endl;
                assert(false);
                continue;
            } else if ((wireMatPos  >  -1) &&
                       (extremeRPos == -1)) {
                extremeRPos = m;
            } else if (extremeRPos > -1) {
                cerr << "WARNING @ WireGroup: ";
                cerr << " Wire: " << wires_.size()+1;
                cerr << ". Two node materials together. ";
                cerr << "Ignoring second.";
                cerr << endl;
                assert(false);
                continue;
            }
        } else if (mats.get(matIds[m])->is<PMWire>()) {
            if (wireMatPos == -1) {
                wireMatPos = m;
            } else if (wireMatPos > -1) {
                cerr << "WARNING @ WireGroup: ";
                cerr << " Wire: " << wires_.size()+1;
                cerr << ". Too many wire materials. ";
                cerr << "Ignoring last.";
                cerr << endl;
                assert(false);
                continue;
            }
        }
    }
    if (wireMatPos < 0) {
        cerr << "WARNING @ WireGroup: ";
        cerr << " Wire: " << wires_.size()+1;
        cerr << ". No wire material. Ignoring wire.";
        cerr << endl;
        assert(false);
        return;
    }
    wireMat = mats.get(wireMatPos)->castTo<PMWire>();
    if (extremeLPos >= 0) {
        extremeL = mats.get(extremeLPos)->castTo<PMMultiport>();
    }
    if (extremeRPos >= 0) {
        extremeR = mats.get(extremeRPos)->castTo<PMMultiport>();
    }
}

template<class T>
Polyline<T>* WiresGroup<T>::newWire_(const vector<const Line<T>*>& lines,
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
        if (prev->getId() == lines[i]->getV(1)->getId()) {
            revIds_.insert(lines[i]->getId());
        }
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

template class WiresGroup<Real>;
template class WiresGroup<Int >;
