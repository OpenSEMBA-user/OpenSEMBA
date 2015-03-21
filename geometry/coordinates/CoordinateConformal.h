/*
 * CoordinateConformal.h
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_
#define SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_

#include <Coordinate.h>

class CoordinateConformal : public virtual CoordI3 {
public:
    CoordinateConformal();
    CoordinateConformal(const CoordinateId id_,
                        const CVecI3& pos,
                        const CartesianAxis dir, const Real length);
    CoordinateConformal(const CartesianAxis dir, const Real length);
    CoordinateConformal(const CoordinateConformal& rhs);
    virtual ~CoordinateConformal();

    ClassBase* clone() const;

    CoordinateConformal& operator=(const CoordinateConformal& rhs);

    CartesianAxis getDir   () const { return dir_;    }
    Real          getLength() const { return length_; }

    void printInfo() const;

private:
    CartesianAxis dir_;
    Real          length_;
};

typedef CoordinateConformal CoordConf;

#endif /* SRC_COMMON_GEOMETRY_COORDINATES_COORDINATECONFORMAL_H_ */
