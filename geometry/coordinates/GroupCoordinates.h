/*
 * GroupCoordinates.h
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#ifndef COMMON_GEOMETRY_COORDINATES_GROUPCOORDINATES_H_
#define COMMON_GEOMETRY_COORDINATES_GROUPCOORDINATES_H_

#include <map>
#include <set>

using namespace std;

#include "Coordinate.h"
#include "base/group/GroupId.h"

struct lexCompareCoord {
    template<class T, Int D>
    bool operator() (const Coordinate<T,D>* lhs,
                     const Coordinate<T,D>* rhs) const {
        return (lhs->pos() < rhs->pos());
    }
};

template<typename C = Coord>
class GroupCoordinates : public virtual GroupId<C, CoordinateId> {
public:
    USE_GROUP_CONSTRUCTS(GroupCoordinates, C);

    GroupCoordinates(const vector<CVecR3>&);
    GroupCoordinates(const vector<CVecI3>&);
    
    DEFINE_GROUP_CLONE(GroupCoordinates, C);

    USE_GROUP_ASSIGN(C);

    USE_GROUPID_GET(C, CoordinateId);
    const CoordR3* get(const CVecR3& pos) const;
    const CoordI3* get(const CVecI3& pos) const;
    
    USE_GROUPID_ADD(C, CoordinateId);
    C*         add(const CVecR3&        , const bool canOverlap = false);
    vector<C*> add(const vector<CVecR3>&, const bool canOverlap = false);
    virtual C*         add(const CVecI3&        , const bool canOverlap = false);
    virtual vector<C*> add(const vector<CVecI3>&, const bool canOverlap = false);
    
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

#include "GroupCoordinates.hpp"

typedef GroupCoordinates<CoordR3> CoordR3Group;
typedef GroupCoordinates<CoordI3> CoordI3Group;

#endif /* COMMON_GEOMETRY_COORDINATES_GROUPCOORDINATES_H_ */
