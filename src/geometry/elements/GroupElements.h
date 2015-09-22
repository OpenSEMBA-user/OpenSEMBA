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

    GroupElements<E>       getMatId(const MatId matId);
    GroupElements<E>       getMatId(const vector<MatId>& matId);
    GroupElements<const E> getMatId(const MatId matId) const;
    GroupElements<const E> getMatId(const vector<MatId>& matId) const;

    GroupElements<E>       getLayerId(const LayerId layerId);
    GroupElements<E>       getLayerId(const vector<LayerId>& layerId);
    GroupElements<const E> getLayerId(const LayerId layerId) const;
    GroupElements<const E> getLayerId(const vector<LayerId>& layerId) const;

    GroupElements<E>       getMatLayerId(const MatId, const LayerId);
    GroupElements<const E> getMatLayerId(const MatId, const LayerId) const;

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
};

#include "GroupElements.hpp"

typedef GroupElements<ElemR> ElemRGroup;
typedef GroupElements<const ElemR> ConstElemRGroup;

#endif /* COMMON_GEOMETRY_ELEMENTS_GROUPELEMENTS_H_ */
