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
 * GroupElements.h
 *
 *  Created on: Mar 28, 2013
 *      Author: luis
 */

#ifndef COMMON_GEOMETRY_ELEMENTS_GROUPELEMENTS_H_
#define COMMON_GEOMETRY_ELEMENTS_GROUPELEMENTS_H_

#include <vector>
#include <set>
#include <map>

using namespace std;

#include "Element.h"
#include "Node.h"
#include <geometry/elements/Line2.h>
#include <geometry/elements/Triangle3.h>
#include <geometry/elements/Triangle6.h>
#include <geometry/elements/Quadrilateral4.h>
#include <geometry/elements/Tetrahedron4.h>
#include <geometry/elements/Tetrahedron10.h>
#include <geometry/elements/Hexahedron8.h>
#include "Polygon.h"
#include "Polyhedron.h"

#include "base/group/GroupId.h"

typedef pair<const VolR*, UInt> Face;

struct CompVecIds {
    bool operator()(const vector<CoordinateId>& lhs,
                    const vector<CoordinateId>& rhs) const {
        vector<CoordinateId> lhsOrdered, rhsOrdered;
        lhsOrdered = ElementBase::ascendingIdOrder(lhs);
        rhsOrdered = ElementBase::ascendingIdOrder(rhs);
        if (lhsOrdered.size() < rhsOrdered.size()) {
            return true;
        } else if (lhsOrdered.size() > rhsOrdered.size()) {
            return false;
        } else {
            for (UInt i = 0; i < lhsOrdered.size(); i++) {
                if (lhsOrdered[i] < rhsOrdered[i]) {
                    return true;
                }
                if (lhsOrdered[i] > rhsOrdered[i]) {
                    return false;
                }
            }
            return false;
        }
    }
};

typedef map<vector<CoordinateId>,const Elem*,CompVecIds> IndexByVertexId;

template<typename E = Elem>
class GroupElements : public GroupId<E, ElementId> {
public:
    GroupElements() {}
    template<typename E2>
    GroupElements(E2* e)                     : GroupId<E,ElementId>(e) {}
    template<typename E2>
    GroupElements(const std::vector<E2*>& e) : GroupId<E,ElementId>(e) {}
    template<typename E2>
    GroupElements(VectorPtr<E2>&       rhs) : GroupId<E,ElementId>(rhs) {}
    template<typename E2>
    GroupElements(const VectorPtr<E2>& rhs) : GroupId<E,ElementId>(rhs) {}
    GroupElements(VectorPtr<E>&        rhs) : GroupId<E,ElementId>(rhs) {}
    template<typename E2>
    GroupElements(VectorPtr<E2>&& rhs) : GroupId<E,ElementId>(std::move(rhs)) {}
    GroupElements(VectorPtr<E >&& rhs) : GroupId<E,ElementId>(std::move(rhs)) {}
    virtual ~GroupElements() {}

    DEFINE_GROUP_CLONE(GroupElements, E);

    GroupElements& operator=(VectorPtr<E>&);
    GroupElements& operator=(VectorPtr<E>&&);
    GroupElements& operator=(const VectorPtr<E>&);

    bool isLinear() const;

    Group<E>       getMatId(const MatId matId);
    Group<E>       getMatId(const vector<MatId>& matId);
    Group<const E> getMatId(const MatId matId) const;
    Group<const E> getMatId(const vector<MatId>& matId) const;

    Group<E>       getLayerId(const LayerId layerId);
    Group<E>       getLayerId(const vector<LayerId>& layerId);
    Group<const E> getLayerId(const LayerId layerId) const;
    Group<const E> getLayerId(const vector<LayerId>& layerId) const;

    Group<E>       getMatLayerId(const MatId, const LayerId);
    Group<const E> getMatLayerId(const MatId, const LayerId) const;

    vector<ElementId> getIdsWithMaterialId   (const MatId matId) const;
    vector<ElementId> getIdsWithoutMaterialId(const MatId matId) const;
    vector<ElementId> getIdsInsideBound(const BoxR3& bound) const;

    vector<pair<const E*,UInt>> getElementsWithVertex(const CoordinateId) const;
    BoxR3 getBound() const;
    BoxR3 getBound(const vector<Face>& border) const;
    virtual const CoordR3* getClosestVertex(const CVecR3 pos) const;

    void setMatId  (const MatId   newId);
    void setLayerId(const LayerId newId);
    void setMatId  (const ElementId id, const MatId   newMatId);
    void setLayerId(const ElementId id, const LayerId newLayerId);

    map<LayerId, vector<const E*> > separateByLayers() const;
    IndexByVertexId getIndexByVertexId() const;

    void removeMatId(const MatId matId);
    void removeMatId(const vector<MatId>& matId);

    void linearize();

    void printInfo() const;

protected:
    template<class T>
    void reassignPointers(const GroupCoordinates< Coordinate<T,3> >& vNew);

private:
    vector<UInt> getElemsWith_(const vector<MatId>&) const;
    vector<UInt> getElemsWith_(const vector<LayerId>&) const;
    vector<UInt> getElemsWith_(const MatId&, const LayerId&) const;
};

#include "GroupElements.hpp"

typedef GroupElements<ElemR> ElemRGroup;
typedef GroupElements<const ElemR> ConstElemRGroup;

#endif /* COMMON_GEOMETRY_ELEMENTS_GROUPELEMENTS_H_ */
