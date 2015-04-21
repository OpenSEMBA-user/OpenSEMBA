/*
 * MeshStructured.h
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_
#define SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_

#include "Mesh.h"

class MeshUnstructured;

class MeshStructured : public virtual Mesh,
                       public virtual Grid3,
                       public virtual CoordinateGroup<CoordI3>,
                       public virtual ElementsGroup<ElemI>,
                       public virtual LayerGroup<Layer> {
public:
    MeshStructured(const Grid3& grid);
    MeshStructured(const Grid3& grid,
                   const CoordinateGroup<const CoordI3>& cG,
                   const ElementsGroup<const ElemI>& elem,
                   const LayerGroup<const Layer>& layer = LayerGroup<>());
    MeshStructured(const MeshStructured& param);
    virtual ~MeshStructured();

    MeshStructured& operator=(const MeshStructured& rhs);

    DEFINE_CLONE(MeshStructured);

    Grid3&                    grid  () { return *this; }
    CoordinateGroup<CoordI3>& coords() { return *this; }
    ElementsGroup<ElemI>&     elems () { return *this; }
    LayerGroup<>&             layers() { return *this; }

    const Grid3&                    grid  () const { return *this; }
    const CoordinateGroup<CoordI3>& coords() const { return *this; }
    const ElementsGroup<ElemI>&     elems () const { return *this; }
    const LayerGroup<>&             layers() const { return *this; }

    MeshUnstructured* getMeshUnstructured() const;

    template<template<typename> class E>
    ElementsGroup< E<Int> > add(const ElementsGroup<E<Real> >&,
                                const Real tol = Grid3::tolerance);
    template<template<typename> class E>
    ElementsGroup< E<Int> > add(const ElementsGroup<const E<Real> >&,
                                const Real tol = Grid3::tolerance);
    template<template<typename> class E>
    ElementsGroup< E<Int> > add(E<Real>*,
                                const Real tol = Grid3::tolerance);

    void convertToHex(ElementsGroup<const SurfI> surfs);
    void addAsHex(ElementsGroup<const VolR> vols);

    Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Real factor);

    virtual void printInfo() const;
private:
    vector<HexI8*> discretizeWithinBoundary(
            const ElementsGroup<const SurfI>& faces);
    vector<pair<const SurfI*, const SurfI*> > getPairsDefiningVolumeWithin(
            const ElementsGroup<const SurfI>& faces) const;
};

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
