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
                       public virtual GroupCoordinates<CoordI3>,
                       public virtual GroupElements<ElemI>,
                       public virtual GroupLayers<Layer> {
public:
    class ErrorInvalidBoundary : public Error {
    public:
        ErrorInvalidBoundary();
        virtual ~ErrorInvalidBoundary() throw();
    };

    MeshStructured(const Grid3& grid);
    MeshStructured(const Grid3& grid,
                   const GroupCoordinates<const CoordI3>& cG,
                   const GroupElements<const ElemI>& elem,
                   const GroupLayers<const Layer>& layer = GroupLayers<>());
    MeshStructured(const MeshStructured& param);
    virtual ~MeshStructured();

    MeshStructured& operator=(const MeshStructured& rhs);

    DEFINE_CLONE(MeshStructured);

    Grid3&                     grid  () { return *this; }
    GroupCoordinates<CoordI3>& coords() { return *this; }
    GroupElements<ElemI>&      elems () { return *this; }
    GroupLayers<>&             layers() { return *this; }

    const Grid3&                     grid  () const { return *this; }
    const GroupCoordinates<CoordI3>& coords() const { return *this; }
    const GroupElements<ElemI>&      elems () const { return *this; }
    const GroupLayers<>&             layers() const { return *this; }

    MeshUnstructured* getMeshUnstructured() const;
    MeshStructured* getConnectivityMesh() const;

    template<template<typename> class E>
    GroupElements< E<Int> > add(const GroupElements<E<Real> >&,
                                const Real tol = Grid3::tolerance);
    template<template<typename> class E>
    GroupElements< E<Int> > add(const GroupElements<const E<Real> >&,
                                const Real tol = Grid3::tolerance);
    template<template<typename> class E>
    GroupElements< E<Int> > add(E<Real>*,
                                const Real tol = Grid3::tolerance);

    void convertToHex(GroupElements<const SurfI> surfs);
    void addAsHex(GroupElements<const VolR> vols);

    Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Real factor);
    BoxR3 getBoundingBox() const;

    virtual void printInfo() const;
private:
    vector<HexI8*> discretizeWithinBoundary(
            const GroupElements<const SurfI>& faces);
    vector<pair<const SurfI*, const SurfI*> > getPairsDefiningVolumeWithin(
            const GroupElements<const SurfI>& faces) const;
};

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
