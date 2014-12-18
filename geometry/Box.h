/*
 * Box.h
 *
 *  Created on: Dec 18, 2014
 *      Author: luis
 */

#include <limits>
#include <utility>
#include "../math/CartesianVector.h"

#ifndef SRC_COMMON_GEOMETRY_BOX_H_
#define SRC_COMMON_GEOMETRY_BOX_H_

template <class T, int D>
class Box {
    typedef CartesianVector<T,D> CVecTD;
public:
    Box();
    Box(const pair<CVecTD,CVecTD>& boundsMinMax);
    Box(const CVecTD& min, const CVecTD& max);
    virtual ~Box();
    void erase ();
    void set(const pair<CVecTD,CVecTD>& boundsMinMax);
    void scale(const double factor);
    bool operator>(const Box<T,D>& lBoxMin) const;
    bool operator<(const Box<T,D>& lBoxMax) const;
    bool operator<=(const Box<T,D>& lBoxMax) const;
    bool operator>=(const Box<T,D>& lBoxMin) const;
    bool operator==(const Box<T,D>& lBoxMin) const;
    bool isIntersected (const Box<T,D>& lBox)   const;
    bool isInnerPoint(const CVecTD& point) const;
    void operator= (const Box<T,D>& lBoxMin);
    void operator+=(const Box<T,D>& lBoxSource);
    void operator+(const Box<T,D>& lBoxSource);
    void operator<<(const CVecTD &p);
    inline CVecTD getMin(void) const;
    inline CVecTD getMax(void) const;
    inline CVecTD getLength() const;
    void printInfo() const;
private:
    pair<CVecTD, CVecTD> minMax_;
    void setDefaultValues(void){
        minMax_.first.setPlusInfty();
        minMax_.second.setMinusInfty();
    };
};

#include "Box.cpp"
typedef Box<double,3> BoxD3;

#endif /* SRC_COMMON_GEOMETRY_BOX_H_ */
