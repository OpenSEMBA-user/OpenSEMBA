/*
 * Box.hpp
 *
 *  Created on: Dec 18, 2014
 *      Author: luis
 */
#include "Box.h"


template<class T, Int D>
Box<T,D>::Box() {
    setDefaultValues();
}

template<class T, Int D> Box<T,D>::Box(
        const pair<CVecTD, CVecTD>& bounds) {
    set(bounds);
}

template<class T, Int D>
Box<T,D>::Box(const CVecTD& min, const CVecTD& max) {
    min_ = min;
    max_ = max;
}

template<class T, Int D>
Box<T,D>::~Box() {
}

template<class T, Int D>
Box<T,D>& Box<T,D>::operator=(const Box<T,D> &rhs) {
    if (&rhs == this) {
        return *this;
    }
    min_ = rhs.min_;
    max_ = rhs.max_;
    return *this;
}

template<class T, Int D>
bool Box<T,D>::operator>(const Box<T,D> &rhs) const {
    for(Int i = 0; i < D; i++) {
        if ((max_(i) <= rhs.max_(i)) || (min_(i) >= rhs.min_(i))) {
            return false;
        }
    }
    return true;
}

template<class T, Int D>
bool Box<T,D>::operator<(const Box<T,D> &lBoxMax) const {
    return lBoxMax > *this;
}

template<class T, Int D>
bool
Box<T,D>::operator <=(const Box<T,D> &rhs) const {
    for(Int i = 0; i < D; i++) {
        if ((max_(i) > rhs.max_(i)) || (min_(i) < rhs.min_(i))) {
            return false;
        }
    }
    return true;
}

template<class T, Int D>
bool Box<T,D>::operator==(const Box<T,D> &rhs) const {
    if (max_ != rhs.max_) { return false; }
    if (min_ != rhs.min_) { return false; }
    return true;
}

template<class T, Int D>
void Box<T,D>::operator+=(const Box<T,D> &rhs) {
    for(Int i = 0; i < D; i++) {
        if (max_(i) < rhs.max_(i)) {
            max_(i) = rhs.max_(i);
        }
        if (min_(i) > rhs.min_(i)) {
            min_(i) = rhs.min_(i);
        }
    }
}

template<class T, Int D>
void Box<T,D>::operator << (const CVecTD& p) {
    for(Int i = 0; i < D; i++) {
        if (min_(i) > p(i)) {
            min_(i) = p(i);
        }
        if (max_(i) < p(i)) {
            max_(i) = p(i);
        }
    }
}

template<class T, Int D>
void Box<T,D>::operator<<(const Box<T,D>& p) {
    *this << p.min_;
    *this << p.max_;
}

template<class T, Int D>
CartesianAxis Box<T,D>::getDirection() const {
    if (!isLine()) {
        cerr << endl << "ERROR @ Box::getDirection(): "
                << "Box is not a Line" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }
    CartesianAxis res = x;
    for(Int d = 0; d < D; d++) {
        if (MathUtils::notEqual(max_(d),min_(d))) {
            res = CartesianAxis(d);
            break;
        }
    }
    return res;
}

template<class T, Int D>
CartesianAxis Box<T,D>::getNormal() const {
    if (!isSurface()) {
        cerr << endl << "ERROR @ Box::getNormal(): "
                << "Box is not a Surface" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }
    assert(D == 3);
    CartesianAxis res = x;
    for(Int d = 0; d < D; d++) {
        if (MathUtils::equal(max_(d),min_(d))) {
            res = CartesianAxis(d);
            break;
        }
    }
    return res;
}

template<class T, Int D>
inline Box<T,D> Box<T,D>::intersect(const Box<T, D>& rhs) const {
    Box<T,D> res;
    for(Int d = 0; d < D; d++) {
        if (min_(d) > rhs.min_(d)) {
            res.min_(d) = min_(d);
        } else {
            res.min_(d) = rhs.min_(d);
        }
        if (max_(d) < rhs.max_(d)) {
            res.max_(d) = max_(d);
        } else {
            res.max_(d) = rhs.max_(d);
        }
    }
    return res;
}

template<class T, Int D>
bool Box<T,D>::isIntersected(const Box<T,D> &rhs) const {
    for(Int i = 0; i < D; i++) {
        if (max_(i) < rhs.min_(i)) {
            return false;
        }
        if (rhs.max_(i) < min_(i)) {
            return false;
        }
    }
    return true;
}

template<class T, Int D>
bool Box<T,D>::isInnerPoint(const CVecTD& point) const {
    bool isInner = true;
    for(Int dir = 0; dir < D; dir++) {
        isInner &= (point(dir) <= getMax()(dir));
        isInner &= (point(dir) >= getMin()(dir));
    }
    return isInner;
}

template<class T, Int D>
inline CartesianVector<T,D> Box<T,D>::getMin() const {
    return min_;
}

template<class T, Int D>
inline CartesianVector<T,D> Box<T,D>::getMax() const {
    return max_;
}

template<class T, Int D>
inline CartesianVector<T,D> Box<T,D>::getLength() const {
    return (max_ - min_);
}

template<class T, Int D>
inline vector<CartesianVector<T,D> > Box<T,D>::getPos() const {
    assert(D == 3);
    vector<CVecTD> res;
    // TODO: Generalize this...
    if (isPoint()) {
        res.push_back(min_);
        return res;
    } else if (isLine()) {
        res.resize(2);
        res[0] = min_;
        res[1] = min_;
        for(Int d = 0; d < D; d++) {
            if (MathUtils::notEqual(max_(d),min_(d))) {
                res[0](d) = min_(d);
                res[1](d) = max_(d);
            }
        }
    } else if (isSurface()) {
        res.resize(4);
        switch(getNormal()) {
        case x:
            res[0] = CVecTD(min_(x), min_(y), min_(z));
            res[1] = CVecTD(min_(x), max_(y), min_(z));
            res[2] = CVecTD(min_(x), max_(y), max_(z));
            res[3] = CVecTD(min_(x), min_(y), max_(z));
            break;
        case y:
            res[0] = CVecTD(min_(x), min_(y), min_(z));
            res[1] = CVecTD(max_(x), min_(y), min_(z));
            res[2] = CVecTD(max_(x), min_(y), max_(z));
            res[3] = CVecTD(min_(x), min_(y), max_(z));
            break;
        case z:
            res[0] = CVecTD(min_(x), min_(y), min_(z));
            res[1] = CVecTD(max_(x), min_(y), min_(z));
            res[2] = CVecTD(max_(x), max_(y), min_(z));
            res[3] = CVecTD(min_(x), max_(y), min_(z));
            break;
        }
    } else if (isVolume()) {
        res.resize(8);
        res[0] = CVecTD(min_(x), min_(y), min_(z));
        res[1] = CVecTD(max_(x), min_(y), min_(z));
        res[2] = CVecTD(max_(x), max_(y), min_(z));
        res[3] = CVecTD(min_(x), max_(y), min_(z));
        res[4] = CVecTD(min_(x), min_(y), max_(z));
        res[5] = CVecTD(max_(x), min_(y), max_(z));
        res[6] = CVecTD(max_(x), max_(y), max_(z));
        res[7] = CVecTD(min_(x), max_(y), max_(z));
    }
    return res;
}

template<class T, Int D>
inline vector<CartesianVector<T, D> > Box<T,D>::getPosOfBound(
        CartesianAxis d,
        CartesianBound p) const {

    assert(D == 3);
    Int rX = d;
    Int rY = (d+1)%D;
    Int rZ = (d+2)%D;
    vector<CVecTD> res;
    // TODO: Generalize this...
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(L)(rY);
        aux(rZ) = getBound(L)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(L)(rY);
        aux(rZ) = getBound(U)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(U)(rY);
        aux(rZ) = getBound(U)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(U)(rY);
        aux(rZ) = getBound(L)(rZ);
        res.push_back(aux);
    }
    return res;
}

template<class T, Int D>
Box<T,D> Box<T,D>::getBoundAsBox(CartesianAxis d, CartesianBound p) const {
    vector<CartesianVector<T,D>> pos = getPosOfBound(d,p);
    assert(pos.size() == 4);
    Box<T,D> res(pos[0],pos[1]);
    for (UInt i = 2; i < pos.size(); i++) {
        res << pos[i];
    }
    assert(res.isSurface());
    return res;
}

template<class T, Int D>
inline CartesianVector<T,D> Box<T,D>::getBound(CartesianBound p) const {
    if (p == L) {
        return getMin();
    } else {
        return getMax();
    }
}

template<class T, Int D>
vector<Box<T,D>> Box<T,D>::chop(const CVecTD origStep) const {
    static_assert(D == 3, "Chop can't be used for Boxes with D != 3");
    CVecTD length = getLength();
    CVecTD step = origStep;
    for (UInt d = 0; d < D; d++) {
        if (length(d) < origStep(d)) {
            step(d) = length(d);
        }
        if (origStep(d) <= (T) 0) {
            step(d) = length(d);
        }
    }
    CartesianVector<Real,3> minR, maxR, stepR;
    for (UInt d = 0; d < D; d++) {
        stepR(d) = step(d);
        if (stepR(d) == 0.0) {
            minR(d) = (T) min_(d);
            maxR(d) = (T) max_(d);
        } else {
            minR(d) = (T) min_(d);
            minR(d) = floor(minR(d)/stepR(d)) * stepR(d);
            maxR(d) = (T) max_(d);
            maxR(d) = floor(maxR(d)/stepR(d)) * stepR(d);
        }
    }
    Grid<D> grid(Box<Real,3>(minR,maxR), stepR);
    CartesianVector<Int,D> numBoxes = grid.getNumCells();
    vector<Box<T,D>> res;
    res.reserve(numBoxes(x)*numBoxes(y)*numBoxes(z));
    for (Int i = 0; i < numBoxes(x); i++) {
        for (Int j = 0; j < numBoxes(y); j++) {
            for (Int k = 0; k < numBoxes(z); k++) {
                CVecR3 min = grid.getPos(CartesianVector<Int,3>(i,j,k));
                CVecR3 max = grid.getPos(CartesianVector<Int,3>(i+1,j+1,k+1));
                CVecI3 minI, maxI;
                for (UInt d = 0; d < D; d++) {
                    minI(d) = (T) min(d);
                    maxI(d) = (T) max(d);
                }
                res.push_back(Box<T,D>(minI,maxI));
            }
        }
    }
    return res;
}

template<class T, Int D>
void Box<T,D>::set(const pair<CVecTD, CVecTD>& minMax) {
    min_ = minMax.first;
    max_ = minMax.second;
}

template<class T, Int D>
inline Box<T,D>& Box<T,D>::setInfinity() {
    for(Int j = 0; j < D; j++) {
        min_(j) = - numeric_limits<T>::infinity();
        max_(j) = numeric_limits<T>::infinity();
    }
    return *this;
}

template<class T, Int D>
void Box<T,D>::scale(const Real factor) {
    if (is_floating_point<T>::value) {
        min_ *= factor;
        max_ *= factor;
    }
}

template<class T, Int D>
void Box<T,D>::printInfo() const {
    cout<< "Box info" << endl;
    cout<< "Min: " << min_.toStr() << endl;
    cout<< "Max: " << max_.toStr() << endl;
    cout<< endl;
}

template<class T, Int D>
UInt Box<T,D>::numberOfDifferentCoords() const {
    UInt res = 0;
    for(Int d = 0; d < D; d++) {
        if (MathUtils::notEqual(max_(d),min_(d))) {
            res++;
        }
    }
    return res;
}
