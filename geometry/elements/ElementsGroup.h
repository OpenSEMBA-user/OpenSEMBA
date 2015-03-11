/*
 * ElementsGroup.h
 *
 *  Created on: Mar 28, 2013
 *      Author: luis
 */

#ifndef ELEMENTSGROUP_H_
#define ELEMENTSGROUP_H_

#include <vector>
#include <set>

using namespace std;

#include "Element.h"
#include "Lin2.h"
#include "Tri3.h"
#include "Tri6.h"
#include "Quad4.h"
#include "Tet4.h"
#include "Tet10.h"
#include "Hex8.h"
#include "Polygon.h"
#include "Polyhedron.h"

#include "GroupId.h"

template<typename E = ElementBase>
class ElementsGroup : public GroupId<E, ElementId> {
public:
    ElementsGroup();
    ElementsGroup(const vector<E*>&, bool ownership = true);
    ElementsGroup(const Group<E>& rhs);
    virtual ~ElementsGroup();

    ElementsGroup<E>& operator=(const Group<E>& rhs);

    bool isLinear() const;
    bool areTriangles   (const vector<ElementId>& elemIds) const;
    bool areTetrahedrons(const vector<ElementId>& elemIds) const;

    ElementsGroup<E> get(const ElementBase::Type type) const;
    ElementsGroup<E> get(const MatId matId   ) const;
    ElementsGroup<E> get(const LayerId layerId ) const;

    ElementsGroup<E> get(const vector<MatId>&) const;
    ElementsGroup<E> get(const vector<LayerId>&) const;

    ElementsGroup<E> get(const MatId matId, const LayerId layId) const;
    ElementsGroup<E> get(const ElementBase::Type type,
                         const MatId matId,
                         const LayerId layId) const;

    vector<ElementId> getIds   () const;
    vector<ElementId> getHexIds() const;
    vector<ElementId> getIdsWithMaterialId   (const MatId matId) const;
    vector<ElementId> getIdsWithoutMaterialId(const MatId matId) const;

    void add(E* newElem , bool newId = false);
    void add(vector<E*>&, bool newId = false);
    vector<ElementId> add(const CoordinateGroup<>& coord,
                          const vector<Hex8>& hex);

    map<LayerId, ElementsGroup<E> > separateLayers() const;
    ElementsGroup<E> removeElementsWithMatId(const MatId matId) const;

    void reassignPointers(const CoordinateGroup<>& vNew);

    void linearize();

    void printInfo() const;
};

#include "ElementsGroup.hpp"

#endif /* ELEMENTSGROUP_H_ */
