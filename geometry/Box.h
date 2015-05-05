/*
 * Box.h
 *
 *  Created on: Dec 18, 2014
 *      Author: luis
 */
#ifndef SRC_COMMON_GEOMETRY_BOX_H_
#define SRC_COMMON_GEOMETRY_BOX_H_

#include <limits>
#include <utility>

using namespace std;

#include "math/CartesianVector.h"
#include "Grid.h"

template<Int D> class Grid;

template <class T, Int D>
class Box {
    typedef CartesianVector<T,D> CVecTD;
public:
    Box();
    Box(const pair<CVecTD,CVecTD>& boundsMinMax);
    Box(const CVecTD& min, const CVecTD& max);
    virtual ~Box();

    Box<T,D>& operator= (const Box<T,D>& lBoxMin);

    bool operator>(const Box<T,D>& lBoxMin) const;
    bool operator<(const Box<T,D>& lBoxMax) const;
    bool operator>=(const Box<T,D>& lBoxMin) const;
    bool operator<=(const Box<T,D>& lBoxMax) const;
    bool operator==(const Box<T,D>& lBoxMin) const;

    void operator+=(const Box<T,D>& lBoxSource);
    void operator+(const Box<T,D>& lBoxSource);

    void operator<<(const CVecTD& p);
    void operator<<(const Box<T,D>& p);

    bool isPoint  () const { return (numberOfDifferentCoords() == 0); }
    bool isLine   () const { return (numberOfDifferentCoords() == 1); }
    bool isSurface() const { return (numberOfDifferentCoords() == 2); }
    bool isVolume () const { return (numberOfDifferentCoords() == 3); }

    CartesianAxis getDirection() const;
    CartesianAxis getNormal   () const;

    Box<T,D> intersect(const Box<T,D>& rhs) const;
    bool isIntersected (const Box<T,D>& lBox) const;
    bool isInnerPoint(const CVecTD& point) const;

    inline CVecTD getMin() const;
    inline CVecTD getMax() const;
    inline CVecTD getLength() const;

    vector<CVecTD> getPos() const;
    vector<CVecTD> getPosOfBound(CartesianAxis d, CartesianBound p) const;
    Box<T,D> getBoundAsBox(CartesianAxis d, CartesianBound p) const;
    CVecTD getBound(CartesianBound p) const;
    vector<Box<T,D>> chop(
            const CartesianVector<T,D> step = CartesianVector<T,D>(1,1,1)) const;

    void set(const pair<CVecTD,CVecTD>& boundsMinMax);
    Box<T,D>& setInfinity();
    void scale(const Real factor);

    void printInfo() const;
private:
    CVecTD min_, max_;

    void setDefaultValues() {
        min_.setPlusInfty();
        max_.setMinusInfty();
    };

    UInt numberOfDifferentCoords() const;
};

template <class T, Int D>
std::ostream& operator<<(ostream& os, const Box<T,D>& rhs) {
   return os << "Min: " << rhs.getMin() << ", Max: " << rhs.getMax();
}

typedef Box<Real,3> BoxR3;
typedef Box<Int ,2> BoxI2;
typedef Box<Int ,3> BoxI3;
typedef Box<Real,3> BBox;

#include "Box.hpp"


#endif /* SRC_COMMON_GEOMETRY_BOX_H_ */
