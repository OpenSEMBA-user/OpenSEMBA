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

typedef pair<const Volume*, UInt> Face;

template<typename E = ElementBase>
class ElementsGroup : public GroupId<E, ElementId> {
public:
    ElementsGroup();
    ElementsGroup(const vector<E*>&);
    ElementsGroup(const Group<E>& rhs);
    virtual ~ElementsGroup();

    ElementsGroup<E>& operator=(const Group<E>& rhs);

    vector<const Element*> get(const Element::Type& type) const;
    vector<const Element*> get(const MatId matId, const LayerId layId) const;
    vector<const Element*> get(const Element::Type& type,
                               const MatId   matId,
                               const LayerId layerId) const;

    void setMaterialIds(
            const vector<ElementId>& ids,
            const MatId newMatId);

    vector<ElementId> getIdsWithMaterialId   (const MatId matId) const;
    vector<ElementId> getIdsWithoutMaterialId(const MatId matId) const;
    vector<ElementId> getIdsInsideBound(const BoxR3& bound, const Element::Type type = Element::undefined) const;

    vector<const Element*> getElementsWithMatId(const vector<MatId>& matId) const;
    vector<const Surface*> getSurfacesWithMatId(const vector<MatId>& matId) const;

    BoxR3 getBound(const vector<Face>& border) const;
    BoxR3 getBound() const;
    virtual const CoordR3* getClosestVertex(const CVecR3 pos) const;

    void add(E* newElem , bool newId = false);
    void add(vector<E*>&, bool newId = false);
    vector<ElementId> add(const CoordinateGroup<>& coord,
                          const vector<Hex8>& hex);

    map<LayerId, vector<const Element*> > separateLayers(
        vector<const Element*>& elem) const;
    ElementsGroup<E> removeElementsWithMatId(const MatId matId) const;

    bool isLinear() const;
    void linearize();

    void printInfo() const;
protected:
    void reassignPointers(const CoordinateGroup<>& vNew);
};

#include "ElementsGroup.hpp"

#endif /* ELEMENTSGROUP_H_ */
