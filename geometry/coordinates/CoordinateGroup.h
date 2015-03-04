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

#include "CoordinateBase.h"
#include "Coordinate.h"

#include "GroupWithIdBase.h"

struct lexCompareCoord {
    bool operator() (const CoordD3* lhs, const CoordD3* rhs) const {
        static const double tolerance = 1e-12;
        for (uint i = 0; i < 3; i++) {
            if (abs(lhs->pos()(i) - rhs->pos()(i)) > tolerance) {
                if (lhs->pos()(i) < rhs->pos()(i)) {
                    return true;
                }
                if (lhs->pos()(i) > rhs->pos()(i)) {
                    return false;
                }
            }
        }
        return false;
    }
    bool operator() (const CoordI3* lhs, const CoordI3* rhs) const {
        for (uint i = 0; i < 3; i++) {
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

template<typename C = CoordinateBase>
class CoordinateGroup : public GroupWithIdBase<C, CoordinateId> {
public:
    CoordinateGroup();
    CoordinateGroup(const vector<C*>&);
    CoordinateGroup(const vector<CVecD3>&);
    CoordinateGroup(const CoordinateGroup<C>& rhs);
    virtual ~CoordinateGroup();
    
    CoordinateGroup<C>& operator=(const CoordinateGroup<C>& rhs);
    
    vector<const CoordD3*> getAll() const;
    
    const CoordD3* get(const CVecD3& pos) const;
    const CoordI3* get(const CVecI3& pos) const;
    
    template<class C2 = CoordinateBase>
    CoordinateGroup<C2> getCoordinatesOf() const;
    
    CoordinateId add(const CVecD3& newPosition);
    vector<CoordinateId> add(const vector<CVecD3>& newPositions,
                             const bool canOverlap = false);
    
    void applyScalingFactor(const double factor);
    
    void printInfo() const;
    
private:
    multiset<const CoordD3*, lexCompareCoord> index_;
};

#include "CoordinateGroup.hpp"

#endif /* COORDINATEGROUP_H_ */
