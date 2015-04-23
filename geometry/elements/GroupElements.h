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
#include "Lin2.h"
#include "Tri3.h"
#include "Tri6.h"
#include "Quad4.h"
#include "Tet4.h"
#include "Tet10.h"
#include "Hex8.h"
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
            }
            return false;
        }
    }
};

typedef map<vector<CoordinateId>,const Elem*,CompVecIds> IndexByVertexId;

template<typename E = Elem>
class GroupElements : public virtual GroupId<E, ElementId> {
public:
    USE_GROUP_CONSTRUCTS(GroupElements, E);

    DEFINE_GROUP_CLONE(GroupElements, E);

    USE_GROUP_ASSIGN(E);

    bool isLinear() const;

    USE_GROUPID_GETGROUPWITH(E, ElementId);
    GroupElements<E> getGroupWith(const MatId matId);
    GroupElements<E> getGroupWith(const vector<MatId>& matId);
    GroupElements<E> getGroupWith(const LayerId layerId);
    GroupElements<E> getGroupWith(const vector<LayerId>& layerId);
    GroupElements<E> getGroupWith(const MatId, const LayerId);
    GroupElements<const E> getGroupWith(const MatId matId) const;
    GroupElements<const E> getGroupWith(const vector<MatId>& matId) const;
    GroupElements<const E> getGroupWith(const LayerId layerId) const;
    GroupElements<const E> getGroupWith(const vector<LayerId>& layerId) const;
    GroupElements<const E> getGroupWith(const MatId, const LayerId) const;

    vector<ElementId> getIdsWithMaterialId   (const MatId matId) const;
    vector<ElementId> getIdsWithoutMaterialId(const MatId matId) const;
    vector<ElementId> getIdsInsideBound(const BoxR3& bound) const;

    vector<pair<const E*, UInt> > getElementsWithVertex(
            const CoordinateId) const;
    BoxR3 getBound() const;
    BoxR3 getBound(const vector<Face>& border) const;
    virtual const CoordR3* getClosestVertex(const CVecR3 pos) const;

    void setMatId  (const MatId   newId);
    void setLayerId(const LayerId newId);
    void setMatId  (const ElementId id, const MatId   newMatId);
    void setLayerId(const ElementId id, const LayerId newLayerId);

    map<LayerId, vector<const E*> > separateByLayers() const;
    IndexByVertexId getIndexByVertexId() const;

    USE_GROUPID_REMOVE(E, ElementId);
    void remove(const MatId matId);
    void remove(const vector<MatId>& matId);

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
