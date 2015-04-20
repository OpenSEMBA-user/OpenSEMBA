/*
 * CoordinateConformal.h
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_
#define SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_

#include <Coordinate.h>

class CoordinateConformal : public virtual Coordinate<Int,3> {
public:
    CoordinateConformal();
    CoordinateConformal(const CoordinateId id_,
                        const CVecI3& pos,
                        const CartesianAxis dir, const Real length);
    CoordinateConformal(const CartesianAxis dir, const Real length);
    CoordinateConformal(const CoordinateConformal& rhs);
    virtual ~CoordinateConformal();

    DEFINE_CLONE(CoordinateConformal);

    CoordinateConformal& operator=(const CoordinateConformal& rhs);

    bool operator==(const CoordinateBase& rhs) const;

    CartesianAxis getDir   () const { return dir_;    }
    Real          getLength() const { return length_; }

    CoordR3* toUnstructured(const Grid3&) const;

    void printInfo() const;

private:
    CartesianAxis dir_;
    Real          length_;
};

typedef CoordinateConformal CoordConf;

#endif /* SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_ */
