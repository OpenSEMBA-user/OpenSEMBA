/*
 * GraphElement.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#ifndef COMMON_GEOMETRY_MAPS_GRAPHELEMENT_H_
#define COMMON_GEOMETRY_MAPS_GRAPHELEMENT_H_


#include <utility>
#include <vector>
using namespace std;

#include "../elements/Element.h"

template<class ELEM, class BOUND>
class GraphElement {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef GraphElement<Elem,Bound> GraphElem;
    typedef GraphElement<Bound,Elem> GraphBound;

    GraphElement(const Elem* elem, const UInt& numBounds = 0);
    GraphElement(const GraphElement&);

    bool visited() const { return vis_;  }
    void markVisited  () { vis_ =  true; }
    void unmarkVisited() { vis_ = false; }

    const Elem* elem() const { return elem_; }

    UInt numBounds() const { return bounds_.size(); }
    vector<const GraphBound*> getBounds() const;
    vector<GraphBound*>       getBounds()    { return bounds_;    }
    const GraphBound* getBound(UInt i) const { return bounds_[i]; }
    GraphBound*       getBound(UInt i)       { return bounds_[i]; }
    void setBounds(GraphBound* bound);
    void setBounds(vector<GraphBound*> bounds) { bounds_ = bounds; }
    void setBound(UInt i, GraphBound* bound)   { bounds_[i] = bound;       }
    void addBound(GraphBound* bound)           { bounds_.push_back(bound); }

    UInt numNeighbors() const { return neighbors_.size(); }
    const GraphElem* getNeighbor(UInt i) const { return neighbors_[i]; }
    GraphElem*       getNeighbor(UInt i)       { return neighbors_[i]; }

    UInt numBoundNeighbors(UInt i) const { return boundNeighbors_[i].size(); }
    const GraphElem* getBoundNeighbor(UInt i, UInt j) const;
    GraphElem*       getBoundNeighbor(UInt i, UInt j);

    void constructNeighbors();

    void printInfo() const;

private:
    bool vis_;

    const Elem* elem_;
    vector<GraphBound*>        bounds_;
    vector<GraphElem*>         neighbors_;
    vector<vector<GraphElem*>> boundNeighbors_;
};

#include "GraphElement.hpp"

#endif /* COMMON_GEOMETRY_MAPS_GRAPHELEMENT_H_ */
