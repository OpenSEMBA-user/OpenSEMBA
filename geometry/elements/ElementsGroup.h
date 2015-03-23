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
#include "Node.h"
#include "Lin2.h"
#include "Tri3.h"
#include "Tri6.h"
#include "Quad4.h"
#include "Tet4.h"
#include "Tet10.h"
#include "Hex8.h"
#include "Polygon.h"
#include "Polyhedron.h"

#include "../../base/GroupId.h"

typedef pair<const VolR*, UInt> Face;

template<typename E = Elem>
class ElementsGroup : public GroupId<E, ElementId> {
public:
    ElementsGroup();
    template<typename E2>
    ElementsGroup(const vector<E2*>& elems, bool ownership = true);
    template<typename E2>
    ElementsGroup(const Group<E2>& rhs);
    virtual ~ElementsGroup();

    template<typename E2>
    ElementsGroup<E>& operator=(const Group<E2>& rhs);

    bool isLinear() const;

    ElementsGroup<E> get(const vector<ElementId>&) const;
    ElementsGroup<E> get(const MatId matId) const;
    ElementsGroup<E> get(const vector<MatId>& matId) const;
    ElementsGroup<E> get(const LayerId layerId) const;
    ElementsGroup<E> get(const vector<LayerId>& layerId) const;
    ElementsGroup<E> get(const MatId, const LayerId) const;

    vector<ElementId> getIdsWithMaterialId   (const MatId matId) const;
    vector<ElementId> getIdsWithoutMaterialId(const MatId matId) const;
    vector<ElementId> getIdsInsideBound(const BoxR3& bound) const;

    vector<pair<const E*, UInt> > getElementsWithVertex(
            const CoordinateId) const;
    BoxR3 getBound() const;
    BoxR3 getBound(const vector<Face>& border) const;
    virtual const CoordR3* getClosestVertex(const CVecR3 pos) const;

    void setMatId  (const ElementId id, const MatId   newMatId);
    void setLayerId(const ElementId id, const LayerId newLayerId);

    map<LayerId, ElementsGroup<E> > separateByLayers() const;
    ElementsGroup<E> removeElementsWithMatId(const MatId matId) const;

    void linearize();

    void printInfo() const;

protected:
    template<class T>
    void reassignPointers(const CoordinateGroup< Coordinate<T,3> >& vNew);
};

#include "ElementsGroup.hpp"

typedef ElementsGroup<ElemR> ElemRGroup;

#endif /* ELEMENTSGROUP_H_ */
