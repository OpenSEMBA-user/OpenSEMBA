/*
 * GraphBase.h
 *
 *  Created on: 17/5/2015
 *      Author: Daniel
 */

#ifndef COMMON_GEOMETRY_MAPS_GRAPHBASE_H_
#define COMMON_GEOMETRY_MAPS_GRAPHBASE_H_

#include <queue>
#include <map>
#include <utility>
#include <vector>
using namespace std;

#include "base/group/Group.h"

#include "GraphElement.h"

template<class ELEM, class BOUND>
class GraphBase {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef GraphElement<Elem,Bound> GraphElem;
    typedef GraphElement<Bound,Elem> GraphBound;

    GraphBase();
    virtual ~GraphBase();

    GraphBase& operator=(const GraphBase&);

    virtual GraphBase& init(const Group<const Elem>&  elems,
                            const Group<const Bound>& bounds) = 0;

    UInt numElems () const { return elems_.size();  }
    UInt numBounds() const { return bounds_.size(); }

    const GraphElem*  elem (UInt i) const { return elems_ [i]; }
    GraphElem*        elem (UInt i)       { return elems_ [i]; }
    const GraphBound* bound(UInt i) const { return bounds_[i]; }
    GraphBound*       bound(UInt i)       { return bounds_[i]; }

    void resetVisited();
    vector<vector<const Elem*>> getConnectedComponents();

    void printInfo() const;

protected:
    vector<GraphElem* > elems_;
    vector<GraphBound*> bounds_;
};

#include "GraphBase.hpp"

#endif /* COMMON_GEOMETRY_MAPS_GRAPHBASE_H_ */
