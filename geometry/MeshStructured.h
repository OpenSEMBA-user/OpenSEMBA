/*
 * MeshStructured.h
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_
#define SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_

#include "Mesh.h"
#include "MeshUnstructured.h"

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

    Grid3&                    grid  () { return *this; }
    CoordinateGroup<CoordI3>& coords() { return *this; }
    ElementsGroup<ElemI>&     elems () { return *this; }

    const Grid3&                    grid  () const { return *this; }
    const CoordinateGroup<CoordI3>& coords() const { return *this; }
    const ElementsGroup<ElemI>&     elems () const { return *this; }

    vector<BoxR3> getRectilinearHexesInsideRegion(
            const ElementsGroup<ElemR>& region) const;

    MeshUnstructured* getMeshUnstructured() const;

    template<template<typename> class E>
    ElementsGroup< E<Int> > add(const ElementsGroup< E<Real> >&,
                                const Real tol = Grid3::tolerance);

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

#include "MeshStructured.hpp"

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
