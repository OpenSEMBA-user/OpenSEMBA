/*
 * CoordinateGroup.h
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#ifndef COORDINATEGROUP_H_
#define COORDINATEGROUP_H_

#include "Coordinate.h"
#include <map>
#include <set>

struct lexCompareCoord {
	static const double tolerance = 1e-12;
    bool
     operator() (
      const CoordD3* lhs,
      const CoordD3* rhs) const {
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
};

class CoordinateGroup {
public:
	CoordinateGroup();
	virtual ~CoordinateGroup();
	CoordinateGroup(const vector<Coordinate<double,3> >&);
	CoordinateGroup&
	 operator=(const CoordinateGroup& rhs);
	long unsigned int
	 size() const {return coord.size();}
	const CoordD3*
	 getPtrToId(const uint id) const;
	const CoordD3*
	 operator()(const uint id) const;
	const CoordD3*
	 get(const CVecD3& pos) const;
	void
	 applyScalingFactor(const double factor);
	void
	 add(const vector<CVecD3>& newPositions, const bool canOverlap = false);
	void
	 add(const CVecD3& newPosition);
	void
	 printInfo() const;
private:
	map<uint, CoordD3*> coord;
	multiset<CoordD3*, lexCompareCoord> index;
};

#endif /* COORDINATEGROUP_H_ */
