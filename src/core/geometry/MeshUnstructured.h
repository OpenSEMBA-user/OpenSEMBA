// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * Mesh.h
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */

#ifndef MESHUNSTRUCTURED_H_
#define MESHUNSTRUCTURED_H_

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

#include "graphs/GraphVertices.h"
#include "graphs/MapGroup.h"
#include "Grid.h"

#include "Mesh.h"

class MeshStructured;

class MeshUnstructured : public virtual Mesh,
                         public virtual GroupCoordinates<CoordR3>,
                         public virtual GroupElements<ElemR>,
                         public virtual GroupLayers<Layer> {
public:
    MeshUnstructured();
    MeshUnstructured(const GroupCoordinates<const CoordR3>& cG,
                     const GroupElements<const ElemR>& elem,
                     const GroupLayers<const Layer>& = GroupLayers<const Layer>());
    MeshUnstructured(const MeshUnstructured& param);
    virtual ~MeshUnstructured();

    MeshUnstructured& operator=(const MeshUnstructured& rhs);

    DEFINE_CLONE(MeshUnstructured);

    GroupCoordinates<CoordR3>& coords() { return *this; }
    GroupElements<ElemR>&      elems () { return *this; }
    GroupLayers<Layer>&        layers() { return *this; }

    const GroupCoordinates<CoordR3>& coords() const { return *this; }
    const GroupElements<ElemR>&      elems () const { return *this; }
    const GroupLayers<Layer>&        layers() const { return *this; }

    MeshStructured* getMeshStructured(const Grid3& grid,
                                      const Real tol = Grid3::tolerance) const;
    MeshUnstructured* getConnectivityMesh() const;

    vector<Face> getBorderWithNormal(const vector<Face>& border,
                                     const CVecR3& normal);

    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    GroupElements<const SurfR> getMaterialBoundary(const MatId   matId,
                                                   const LayerId layId) const;
    GroupElements<const SurfR> getSurfsMatching(
            const vector<Face>& faces) const;
    GroupElements<const Triangle> convertToTri(
            const GroupElements<const ElemR>& region,
            bool ignoreTets) const;
    vector<Face> getInternalBorder(
            const GroupElements<const ElemR>& region) const;
    vector<Face> getExternalBorder(
            const GroupElements<const ElemR>& region) const;
    GroupElements<const VolR> getAdjacentRegion(
            const GroupElements<const VolR>& region) const;
    Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Real factor);
    BoxR3 getBoundingBox() const;

    void printInfo() const;
protected:
    vector<Face> getTetInternalBorder(
            const GroupElements<const Tetrahedron>& tet) const;
    vector<Face> getTriInternalBorder(
            const GroupElements<const Triangle>& tri) const;
};

#endif /* MESHUNSTRUCTURED_H_ */
