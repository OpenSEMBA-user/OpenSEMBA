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

#include "GroupId.h"

struct lexCompareCoord {
    bool operator() (const CoordR3* lhs, const CoordR3* rhs) const {
        static const Real tolerance = 1e-12;
        for (UInt i = 0; i < 3; i++) {
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

template<typename C = CoordinateBase>
class CoordinateGroup : public GroupId<C, CoordinateId> {
public:
    CoordinateGroup();
    CoordinateGroup(const vector<C*>&);
    CoordinateGroup(const vector<CVecR3>&);
    CoordinateGroup(const Group<C>& rhs);
    virtual ~CoordinateGroup();
    
    CoordinateGroup<C>& operator=(const Group<C>& rhs);
    
    const CoordR3* get(const CVecR3& pos) const;
    const CoordI3* get(const CVecI3& pos) const;
    
    void add(C* newElem , bool newId = false);
    void add(vector<C*>&, bool newId = false);
    C*         add(const CVecR3&        , const bool canOverlap = false);
    vector<C*> add(const vector<CVecR3>&, const bool canOverlap = false);
    
    void applyScalingFactor(const Real factor);
    
    void printInfo() const;
    
private:
    multiset<const CoordR3*, lexCompareCoord> index_;
    
    void buildIndex(const vector<C*>& coords);
};

#include "CoordinateGroup.hpp"

#endif /* COORDINATEGROUP_H_ */
