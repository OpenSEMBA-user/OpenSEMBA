/*
 * GraphVertices.h
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#ifndef COMMON_GEOMETRY_MAPS_GRAPHVERTICES_H_
#define COMMON_GEOMETRY_MAPS_GRAPHVERTICES_H_

#include "GraphBase.h"

template<class ELEM, class BOUND>
class GraphVertices : public GraphBase<ELEM, BOUND> {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef GraphElement<Elem,Bound> GraphElem;
    typedef GraphElement<Bound,Elem> GraphBound;

    GraphVertices();
    GraphVertices(const Group<const Elem>&  elems,
                  const Group<const Bound>& bounds);
    virtual ~GraphVertices();

    GraphVertices<Elem,Bound>& init(const Group<const Elem>&  elems,
                                    const Group<const Bound>& bounds);

    void splitBound(UInt i);
};

#include "GraphVertices.hpp"

#endif /* COMMON_GEOMETRY_MAPS_GRAPHVERTICES_H_ */
