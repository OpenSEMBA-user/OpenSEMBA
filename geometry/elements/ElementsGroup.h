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

using namespace std;

class ElementsGroup {
public:
    vector<Element*> element;
    // Line storage variables.
    vector<Line*> lineElement;
    vector<Lin2> lin2;
    // Surface storage variables.
    vector<Tri*> tri;
    vector<Tri3> tri3;
    vector<Tri6> tri6;
    vector<Quad4> quad4;
    // Volume storage variables.
    vector<Tet*> tet;
    vector<Tet4> tet4;
    vector<Tet10> tet10;
    vector<Hex8> hex8;
    uint offsetId, offsetIdLin, offsetIdTri, offsetIdTet;
    ElementsGroup();
    ElementsGroup(const ElementsGroup& elem);
    virtual ~ElementsGroup();
    ElementsGroup(
            const vector<Tri3>&,
            const vector<Quad4>&);
    ElementsGroup(
            const vector<Lin2>&);
    ElementsGroup(
            const vector<Lin2>&,
            const vector<Tri3>&, const vector<Tri6>&,
            const vector<Tet4>&, const vector<Tet10>&);
    ElementsGroup(
            const vector<Lin2>&,
            const vector<Tri3>&, const vector<Tri6>&,
            const vector<Tet4>&, const vector<Tet10>&,
            const vector<Hex8>&);
    ElementsGroup(
            const vector<Tri3>&, const vector<Tri6>&);
    ElementsGroup&
    operator=(const ElementsGroup& param);
    vector<ElementId> add(
            const CoordinateGroup<>& coord,
            const vector<Hex8>& hex);
    void reassignPointers(const CoordinateGroup<>& vNew);
    vector<ElementId> getIds(const vector<const Element*>& list) const;
    uint nSurfaceElements() const;
    uint
    nVolumeElements() const;
    Element* getPtrToId(const uint id);
    const Element* getPtrToId(const uint id) const;
    const Tri* getTriPtrToId(const uint id) const;
    const Tet* getTetPtrToId(const uint id) const;
    void linearize();
    bool isLinear() const;
    bool areTetrahedrons(const vector<uint>& elemIds) const;
    bool areTriangles(const vector<uint>& elemIds) const;
    vector<uint> getHexIds() const;
    vector<uint> getIdsWithMaterialId(const uint matId) const;
    vector<uint> getIdsWithoutMaterialId(const uint matId) const;
    vector<const Element*> get(
            const Element::Type& type,
            const uint matId,
            const uint layerId) const;
    vector<const Element*> get(
            const Element::Type& type) const;
    vector<const Element*>
    getElementsWithMatId(
            const vector<uint>& matId) const;
    vector<const Surface*>
    getSurfacesWithMatId(const vector<uint>& matId) const;
    vector<const Element*>
    get(
            const uint matId, const uint layId) const;
    ElementsGroup
    removeElementsWithMatId(
            const uint matId) const;
    map<LayerId, vector<const Element*> >
    separateLayers(vector<const Element*>& elem) const;
    void
    printInfo() const;
private:
    void
    checkIdsAreConsecutive();
    void
    updatePointers();
};
#endif /* ELEMENTSGROUP_H_ */
