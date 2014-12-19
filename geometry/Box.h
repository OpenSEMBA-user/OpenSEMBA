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
    void set(const pair<CVecTD,CVecTD>& boundsMinMax);
    Box<T,D>& setInfinity();
    void scale(const double factor);
    bool operator>(const Box<T,D>& lBoxMin) const;
    bool operator<(const Box<T,D>& lBoxMax) const;
    bool operator<=(const Box<T,D>& lBoxMax) const;
    bool operator>=(const Box<T,D>& lBoxMin) const;
    bool operator==(const Box<T,D>& lBoxMin) const;
    bool isIntersected (const Box<T,D>& lBox)   const;
    bool isInnerPoint(const CVecTD& point) const;
    Box<T,D>& operator= (const Box<T,D>& lBoxMin);
    void operator+=(const Box<T,D>& lBoxSource);
    void operator+(const Box<T,D>& lBoxSource);
    void operator<<(const CVecTD& p);
    void operator<<(const Box<T,D>& p);
    inline CVecTD getMin(void) const;
    inline CVecTD getMax(void) const;
    inline CVecTD getLength() const;
    void printInfo() const;
private:
    CVecTD min_, max_;
    void setDefaultValues(void){
        min_.setPlusInfty();
        max_.setMinusInfty();
    };
};

#include "Box.hpp"

typedef Box<double,3> BoxD3;

#endif /* SRC_COMMON_GEOMETRY_BOX_H_ */
