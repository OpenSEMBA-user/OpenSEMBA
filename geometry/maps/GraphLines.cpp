/*
 * GraphLines.cpp
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#include "GraphLines.h"

template<class T>
GraphLines<T>::GraphLines() {

}

template<class T>
GraphLines<T>::GraphLines(const GroupElements<const Line<T>>& lines) {
    init_(lines);
}

template<class T>
GraphLines<T>::~GraphLines() {
    for (UInt i = 0; i < coords_.size(); i++) {
        delete coords_[i];
    }
    for (UInt i = 0; i < lines_.size(); i++) {
        delete lines_[i];
    }
}

template<class T>
void GraphLines<T>::splitNode(UInt i) {
    vector<const VertexLine<T>*> oldAdj = coords_[i]->getLines();
    if (oldAdj.empty()) {
        return;
    }
    coords_[i]->setLines(oldAdj[0]);
    mapLines_[oldAdj[0]->line()->getId()]->constructLines();
    for (UInt i = 0; i < oldAdj.size(); i++) {
        coords_.push_back(new VertexCoord<T>(*coords_[i]));
        coords_.back()->setLines(oldAdj[i]);
        mapLines_[oldAdj[i]->line()->getId()]->constructLines();
    }
}

template<class T>
void GraphLines<T>::init_(const GroupElements<const Line<T>>& lines) {
    LayerId layId;
    const Coordinate<T,3>* v[2];
    VertexLine<T>* segmentPtr;
    VertexCoord<T>* nodePtr;
    map<pair<LayerId,CoordinateId>, VertexCoord<T>*> map;
    for (UInt s = 0; s < lines.size(); s++) {
        layId = lines(s)->getLayerId();
        v[0] = lines(s)->getVertex(0);
        v[1] = lines(s)->getVertex(1);
        if(v[0]->getId() == v[1]->getId()) {
            continue;
        }
        segmentPtr = new VertexLine<T>(lines(s));
        mapLines_[segmentPtr->line()->getId()] = segmentPtr;
        lines_.push_back(segmentPtr);
        for(Int i = 0; i < 2; i++) {
            if (map.count(make_pair(layId,v[i]->getId())) == 0) {
                nodePtr = new VertexCoord<T>(v[i]);
                coords_.push_back(nodePtr);
                map[make_pair(layId,v[i]->getId())] = nodePtr;
            }
            nodePtr = map[make_pair(layId,v[i]->getId())];
            segmentPtr->setCoord(i, nodePtr);
            nodePtr->addLine(segmentPtr);
        }
    }
    map.clear();
    for (UInt i = 0; i < lines_.size(); i++) {
        lines_[i]->constructLines();
    }
}

template<class T>
void GraphLines<T>::printInfo() const {
    cout << "--- GraphLines Info ---" << endl;
    cout << "Coords:" << endl;
    for (UInt i = 0; i < coords_.size(); i++) {
        coords_[i]->printInfo();
    }
    cout << "Lines:" << endl;
    for (UInt i = 0; i < lines_.size(); i++) {
        lines_[i]->printInfo();
    }
}

template class GraphLines<Real>;
template class GraphLines<Int >;
