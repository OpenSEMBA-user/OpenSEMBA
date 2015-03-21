/*
 * MeshStructured.h
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_
#define SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_

#include "Mesh.h"

class MeshStructured : public virtual Mesh,
                       public Grid3,
                       public CoordinateGroup<CoordI3>,
                       public ElementsGroup<ElemI> {
public:
    MeshStructured();
    MeshStructured(const Grid3& grid,
                   const CoordinateGroup<CoordI3>& cG,
                   const ElementsGroup<ElemI>& elem,
                   const LayerGroup<>& layer = LayerGroup<>());
    MeshStructured(const MeshStructured& param);
    virtual ~MeshStructured();

    MeshStructured& operator=(const MeshStructured& rhs);

    ClassBase* clone() const;

    CoordinateGroup<CoordI3>& coords() { return *this; }
    ElementsGroup<ElemI>&     elems () { return *this; }

    const CoordinateGroup<CoordI3>& coords() const { return *this; }
    const ElementsGroup<ElemI>&     elems () const { return *this; }

    vector<BoxR3> getRectilinearHexesInsideRegion(
            const ElementsGroup<ElemR>& region) const;

    const Grid3& getGrid() const;
    void setGrid(const Grid3&);

    template<template<typename> class E>
    ElementsGroup< E<Int> > add(const ElementsGroup< E<Real> >& rhs) {
        vector< E<Int>* > elems;
        ElemI* elem;
        elems.reserve(rhs.size());
        for(UInt i = 0; i < rhs.size(); i++) {
            elem = rhs(i)->toStructured(*this, *this);
            if (elem != NULL) {
                ElementsGroup<ElemI>::add(elem);
                elems.push_back(elem->castTo< E<Int> >());
            }
        }
        return ElementsGroup< E<Int> >(elems, false);
    }

    vector<BoxR3> discretizeWithinBoundary(
            const MatId matId,
            const LayerId layId) const;

    void applyScalingFactor(const Real factor);

    virtual void printInfo() const;
private:
    vector<BoxR3> discretizeWithinBoundary(
            const Grid3* grid,
            const ElementsGroup<SurfR>& faces) const;
    vector<BoxR3> discretizeWithinBoundary(
            const ElementsGroup<SurfI>& faces) const;
    vector<pair<const SurfI*, const SurfI*> > getPairsDefiningVolumeWithin(
            const ElementsGroup<SurfI>& faces) const;
};

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
