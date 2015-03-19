/*
 * MeshStructured.h
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_
#define SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_

#include "elements/ElementsGroup.h"
#include "layers/LayerGroup.h"
#include "Grid.h"

class MeshStructured : public Grid3,
                       public CoordinateGroup<CoordI3>,
                       public ElementsGroup<ElemI>,
                       public LayerGroup<> {
public:
    MeshStructured();
    MeshStructured(const Grid3& grid,
                   const CoordinateGroup<CoordI3>& cG,
                   const ElementsGroup<ElemI>& elem,
                   const LayerGroup<>& layer = LayerGroup<>());
    MeshStructured(const MeshStructured& param);
    virtual ~MeshStructured();

    MeshStructured& operator=(const MeshStructured& rhs);

    CoordinateGroup<CoordI3>& coords() { return *this; }
    ElementsGroup<ElemI>&     elems () { return *this; }
    LayerGroup<>&             layers() { return *this; }

    const CoordinateGroup<CoordI3>& coords() const { return *this; }
    const ElementsGroup<ElemI>&     elems () const { return *this; }
    const LayerGroup<>&             layers() const { return *this; }

    vector<BoxR3> getRectilinearHexesInsideRegion(
            const ElementsGroup<ElemR>& region) const;

    vector<BoxR3> discretizeWithinBoundary(
            const UInt matId,
            const UInt layId) const;

    const Grid3& getGrid() const;
    void setGrid(const Grid3&);

    template<template<typename> class E>
    ElementsGroup< E<Int> > add(const ElementsGroup< E<Real> >& rhs) {
        vector< E<Int>* > elems;
        E<Int>* elem;
        elems.reserve(rhs.size());
        for(UInt i = 0; i < rhs.size(); i++) {
            elem = rhs(i)->toStructured(*this, *this);
            if (elem != NULL) {
                ElementsGroup<ElemI>::add(elem->template castTo<ElemI>(),true);
                elems.push_back(elem);
            }
        }
        return ElementsGroup< E<Int> >(elems, false);
    }

    void applyScalingFactor(const Real factor);
    virtual void printInfo() const;
};

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
