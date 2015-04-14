/*
 * CoordinateGroup.h
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#ifndef COORDINATEGROUP_H_
#define COORDINATEGROUP_H_

#include <map>
#include <set>

using namespace std;

#include "Coordinate.h"
#include "../../base/group/GroupId.h"

struct lexCompareCoord {
    bool operator() (const CoordR3* lhs, const CoordR3* rhs) const {
        for (UInt i = 0; i < 3; i++) {
            if (MathUtils::lower   (lhs->pos()(i),rhs->pos()(i),lhs->norm())) {
                return true;
            }
            if (MathUtils::greather(lhs->pos()(i),rhs->pos()(i),lhs->norm())) {
                return false;
            }
        }
        return false;
    }
    bool operator() (const CoordI3* lhs, const CoordI3* rhs) const {
        for (UInt i = 0; i < 3; i++) {
            if (lhs->pos()(i) < rhs->pos()(i)) {
                return true;
            }
            if (lhs->pos()(i) > rhs->pos()(i)) {
                return false;
            }
        }
        return false;
    }
};

template<typename C = Coord>
class CoordinateGroup : public virtual GroupId<C, CoordinateId> {
public:
    USE_GROUP_CONSTRUCTS(CoordinateGroup, C);

    CoordinateGroup(const vector<CVecR3>&);
    CoordinateGroup(const vector<CVecI3>&);
    
    DEFINE_GROUP_CLONE(CoordinateGroup, C);

    USE_GROUP_ASSIGN(C);

    USE_GROUPID_GET(C, CoordinateId);
    const CoordR3* get(const CVecR3& pos) const;
    const CoordI3* get(const CVecI3& pos) const;
    
    USE_GROUPID_ADD(C, CoordinateId);
    C*         add(const CVecR3&        , const bool canOverlap = false);
    vector<C*> add(const vector<CVecR3>&, const bool canOverlap = false);
    C*         add(const CVecI3&        , const bool canOverlap = false);
    vector<C*> add(const vector<CVecI3>&, const bool canOverlap = false);
    
    void applyScalingFactor(const Real factor);
    
    void printInfo() const;
    
protected:
    void construct();
    void destruct ();

    void postprocess(const UInt i);

private:
    multiset<const CoordR3*, lexCompareCoord> indexUnstr_;
    multiset<const CoordI3*, lexCompareCoord> indexStr_;
};

#include "CoordinateGroup.hpp"

typedef CoordinateGroup<CoordR3> CoordR3Group;

#endif /* COORDINATEGROUP_H_ */
