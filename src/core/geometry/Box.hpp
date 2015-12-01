// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include "Box.h"

#include <limits>

namespace SEMBA {
namespace Geometry {

template<class T, Size D>
Box<T,D>::Box() {
    setDefaultValues();
}

template<class T, Size D>
Box<T,D>::Box(
        const std::pair<CVecTD, CVecTD>& bounds) {
    set(bounds);
}

template<class T, Size D>
Box<T,D>::Box(const CVecTD& min, const CVecTD& max) {
    min_ = min;
    max_ = max;
}

template<class T, Size D>
Box<T,D>::~Box() {
}

template<class T, Size D>
Box<T,D>& Box<T,D>::operator=(const Box<T,D> &rhs) {
    if (&rhs == this) {
        return *this;
    }
    min_ = rhs.min_;
    max_ = rhs.max_;
    return *this;
}

template<class T, Size D>
bool Box<T,D>::operator>(const Box<T,D> &rhs) const {
    for(Size i = 0; i < D; i++) {
        if ((max_(i) <= rhs.max_(i)) || (min_(i) >= rhs.min_(i))) {
            return false;
        }
    }
    return true;
}

template<class T, Size D>
bool Box<T,D>::operator<(const Box<T,D> &lBoxMax) const {
    return lBoxMax > *this;
}

template<class T, Size D>
bool
Box<T,D>::operator <=(const Box<T,D> &rhs) const {
    for(Size i = 0; i < D; i++) {
        if ((max_(i) > rhs.max_(i)) || (min_(i) < rhs.min_(i))) {
            return false;
        }
    }
    return true;
}

template<class T, Size D>
bool Box<T,D>::operator==(const Box<T,D> &rhs) const {
    if (max_ != rhs.max_) { return false; }
    if (min_ != rhs.min_) { return false; }
    return true;
}

template<class T, Size D>
void Box<T,D>::operator+=(const Box<T,D> &rhs) {
    for(Size i = 0; i < D; i++) {
        if (min_(i) > rhs.min_(i)) {
            min_(i) = rhs.min_(i);
        }
        if (max_(i) < rhs.max_(i)) {
            max_(i) = rhs.max_(i);
        }
    }
}

template<class T, Size D>
void Box<T,D>::operator << (const CVecTD& p) {
    for(Size i = 0; i < D; i++) {
        if (min_(i) > p(i)) {
            min_(i) = p(i);
        }
        if (max_(i) < p(i)) {
            max_(i) = p(i);
        }
    }
}

template<class T, Size D>
void Box<T,D>::operator<<(const Box<T,D>& p) {
    *this << p.min_;
    *this << p.max_;
}

template<class T, Size D>
Math::Constants::CartesianAxis Box<T,D>::getDirection() const {
    if (!isLine()) {
        throw Error::Box::NotLine();
    }
    Math::Constants::CartesianAxis res = Math::Constants::x;
    for(Size d = 0; d < D; d++) {
        if (Math::Util::notEqual(max_(d),min_(d))) {
            res = Math::Constants::CartesianAxis(d);
            break;
        }
    }
    return res;
}

template<class T, Size D>
Math::Constants::CartesianAxis Box<T,D>::getNormal() const {
    if (!isSurface()) {
        throw Error::Box::NotSurface();
    }
    assert(D == 3);
    Math::Constants::CartesianAxis res = Math::Constants::x;
    for(Size d = 0; d < D; d++) {
        if (Math::Util::equal(max_(d),min_(d))) {
            res = Math::Constants::CartesianAxis(d);
            break;
        }
    }
    return res;
}

template<class T, Size D>
inline Box<T,D> Box<T,D>::intersect(const Box<T, D>& rhs) const {
    Box<T,D> res;
    for(Size d = 0; d < D; d++) {
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

template<class T, Size D>
bool Box<T,D>::isIntersected(const Box<T,D> &rhs) const {
    for(Size i = 0; i < D; i++) {
        if (max_(i) < rhs.min_(i)) {
            return false;
        }
        if (rhs.max_(i) < min_(i)) {
            return false;
        }
    }
    return true;
}

template<class T, Size D>
bool Box<T,D>::isInnerPoint(const CVecTD& point) const {
    bool isInner = true;
    for(Size dir = 0; dir < D; dir++) {
        isInner &= (point(dir) <= getMax()(dir));
        isInner &= (point(dir) >= getMin()(dir));
    }
    return isInner;
}

template<class T, Size D>
inline Math::Vector::Cartesian<T,D> Box<T,D>::getMin() const {
    return min_;
}

template<class T, Size D>
inline Math::Vector::Cartesian<T,D> Box<T,D>::getMax() const {
    return max_;
}

template<class T, Size D>
inline Math::Vector::Cartesian<T,D> Box<T,D>::getLength() const {
    return (max_ - min_);
}

template<class T, Size D>
inline std::vector<Math::Vector::Cartesian<T,D> > Box<T,D>::getPos() const {
    assert(D == 3);
    std::vector<CVecTD> res;
    // TODO: Generalize this...
    if (isPoint()) {
        res.push_back(min_);
        return res;
    } else if (isLine()) {
        res.resize(2);
        res[0] = min_;
        res[1] = min_;
        for(Size d = 0; d < D; d++) {
            if (Math::Util::notEqual(max_(d),min_(d))) {
                res[0](d) = min_(d);
                res[1](d) = max_(d);
            }
        }
    } else if (isSurface()) {
        res.resize(4);
        switch(getNormal()) {
        case Math::Constants::x:
            res[0] = CVecTD(min_(Math::Constants::x),
                            min_(Math::Constants::y),
                            min_(Math::Constants::z));
            res[1] = CVecTD(min_(Math::Constants::x),
                            max_(Math::Constants::y),
                            min_(Math::Constants::z));
            res[2] = CVecTD(min_(Math::Constants::x),
                            max_(Math::Constants::y),
                            max_(Math::Constants::z));
            res[3] = CVecTD(min_(Math::Constants::x),
                            min_(Math::Constants::y),
                            max_(Math::Constants::z));
            break;
        case Math::Constants::y:
            res[0] = CVecTD(min_(Math::Constants::x),
                            min_(Math::Constants::y),
                            min_(Math::Constants::z));
            res[1] = CVecTD(min_(Math::Constants::x),
                            min_(Math::Constants::y),
                            max_(Math::Constants::z));
            res[2] = CVecTD(max_(Math::Constants::x),
                            min_(Math::Constants::y),
                            max_(Math::Constants::z));
            res[3] = CVecTD(max_(Math::Constants::x),
                            min_(Math::Constants::y),
                            min_(Math::Constants::z));
            break;
        case Math::Constants::z:
            res[0] = CVecTD(min_(Math::Constants::x),
                            min_(Math::Constants::y),
                            min_(Math::Constants::z));
            res[1] = CVecTD(max_(Math::Constants::x),
                            min_(Math::Constants::y),
                            min_(Math::Constants::z));
            res[2] = CVecTD(max_(Math::Constants::x),
                            max_(Math::Constants::y),
                            min_(Math::Constants::z));
            res[3] = CVecTD(min_(Math::Constants::x),
                            max_(Math::Constants::y),
                            min_(Math::Constants::z));
            break;
        }
    } else if (isVolume()) {
        res.resize(8);
        res[0] = CVecTD(min_(Math::Constants::x),
                        min_(Math::Constants::y),
                        min_(Math::Constants::z));
        res[1] = CVecTD(max_(Math::Constants::x),
                        min_(Math::Constants::y),
                        min_(Math::Constants::z));
        res[2] = CVecTD(max_(Math::Constants::x),
                        max_(Math::Constants::y),
                        min_(Math::Constants::z));
        res[3] = CVecTD(min_(Math::Constants::x),
                        max_(Math::Constants::y),
                        min_(Math::Constants::z));
        res[4] = CVecTD(min_(Math::Constants::x),
                        min_(Math::Constants::y),
                        max_(Math::Constants::z));
        res[5] = CVecTD(max_(Math::Constants::x),
                        min_(Math::Constants::y),
                        max_(Math::Constants::z));
        res[6] = CVecTD(max_(Math::Constants::x),
                        max_(Math::Constants::y),
                        max_(Math::Constants::z));
        res[7] = CVecTD(min_(Math::Constants::x),
                        max_(Math::Constants::y),
                        max_(Math::Constants::z));
    }
    return res;
}

template<class T, Size D>
inline std::vector<Math::Vector::Cartesian<T, D> > Box<T,D>::getPosOfBound(
        Math::Constants::CartesianAxis d,
        Math::Constants::CartesianBound p) const {

    assert(D == 3);
    Size rX = d;
    Size rY = (d+1)%D;
    Size rZ = (d+2)%D;
    std::vector<CVecTD> res;
    // TODO: Generalize this...
    {
        CVecTD aux;
        aux(rX) = getBound(Math::Constants::CartesianBound(p))(rX);
        aux(rY) = getBound(Math::Constants::L)(rY);
        aux(rZ) = getBound(Math::Constants::L)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(Math::Constants::CartesianBound(p))(rX);
        aux(rY) = getBound(Math::Constants::L)(rY);
        aux(rZ) = getBound(Math::Constants::U)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(Math::Constants::CartesianBound(p))(rX);
        aux(rY) = getBound(Math::Constants::U)(rY);
        aux(rZ) = getBound(Math::Constants::U)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(Math::Constants::CartesianBound(p))(rX);
        aux(rY) = getBound(Math::Constants::U)(rY);
        aux(rZ) = getBound(Math::Constants::L)(rZ);
        res.push_back(aux);
    }
    return res;
}

template<class T, Size D>
Box<T,D> Box<T,D>::getBoundAsBox(Math::Constants::CartesianAxis d,
                                 Math::Constants::CartesianBound p) const {
    std::vector<Math::Vector::Cartesian<T,D>> pos = getPosOfBound(d,p);
    assert(pos.size() == 4);
    Box<T,D> res(pos[0],pos[1]);
    for (Size i = 2; i < pos.size(); i++) {
        res << pos[i];
    }
    assert(res.isSurface());
    return res;
}

template<class T, Size D>
inline Math::Vector::Cartesian<T,D> Box<T,D>::getBound(
        Math::Constants::CartesianBound p) const {
    if (p == Math::Constants::L) {
        return getMin();
    } else {
        return getMax();
    }
}

template<class T, Size D>
std::vector<Box<T,D>> Box<T,D>::chop(const CVecTD origStep) const {
    static_assert(D == 3, "Chop can't be used for Boxes with D != 3");
    CVecTD length = getLength();
    CVecTD step = origStep;
    for (Size d = 0; d < D; d++) {
        if (length(d) < origStep(d)) {
            step(d) = length(d);
        }
        if (origStep(d) <= (T) 0) {
            step(d) = length(d);
        }
    }
    Math::Vector::Cartesian<Math::Real,3> minR, maxR, stepR;
    for (Size d = 0; d < D; d++) {
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
    Grid<D> grid(Box<Math::Real,3>(minR,maxR), stepR);
    return chop(grid);
}

template<class T, Size D>
std::vector<Box<T,D>> Box<T,D>::chop(const Grid<D>& grid) const {
    static_assert(D == 3, "Chop can't be used for Boxes with D != 3");
    // Prepares subgrid with the size of the box preserving grid positions.
    std::vector<Math::Real> pos[D];
    for (Size d = 0; d < D; d++) {
        pos[d] = grid.getPosInRange(d, min_(d), max_(d));
        if (min_(d) != pos[d].front()) {
            std::vector<Math::Real> aux(1,min_(d));
            aux.insert(aux.end(), pos[d].begin(), pos[d].end());
            pos[d] = aux;
        }
        if (max_(d) != pos[d].back()) {
            pos[d].push_back(max_(d));
        }
    }
    Grid<D> subGrid;
    subGrid.setPos(pos);
    //
    Math::Vector::Cartesian<Size,D> numBoxes = subGrid.getNumCells();
    std::vector<Box<T,D>> res;
    res.reserve(numBoxes(Math::Constants::x)*
                numBoxes(Math::Constants::y)*
                numBoxes(Math::Constants::z));
    for (Size i = 0; i < numBoxes(Math::Constants::x); i++) {
        for (Size j = 0; j < numBoxes(Math::Constants::y); j++) {
            for (Size k = 0; k < numBoxes(Math::Constants::z); k++) {
                CVecTD min = subGrid.getPos(CVecTD(i,j,k));
                CVecTD max = subGrid.getPos(CVecTD(i+1,j+1,k+1));
                CVecTD minT, maxT;
                for (Size d = 0; d < D; d++) {
                    if (min(d) >= min_(d)) {
                        minT(d) = (T) min(d);
                    } else {
                        minT(d) = min_(d);
                    }
                    if (max(d) <= max_(d)) {
                        maxT(d) = (T) max(d);
                    } else {
                        maxT(d) = max_(d);
                    }
                }
                res.push_back(Box<T,D>(minT,maxT));
            }
        }
    }
    return res;
}

template<class T, Size D>
void Box<T,D>::set(const std::pair<CVecTD, CVecTD>& minMax) {
    min_ = minMax.first;
    max_ = minMax.second;
}

template<class T, Size D>
inline Box<T,D>& Box<T,D>::setInfinity() {
    for(Size j = 0; j < D; j++) {
        min_(j) = - std::numeric_limits<T>::infinity();
        max_(j) = std::numeric_limits<T>::infinity();
    }
    return *this;
}

template<class T, Size D>
void Box<T,D>::scale(const Math::Real factor) {
    if (std::is_floating_point<T>::value) {
        min_ *= factor;
        max_ *= factor;
    }
}

template<class T, Size D>
void Box<T,D>::printInfo() const {
    std::cout<< "Box info" << std::endl;
    std::cout<< "Min: " << min_.toStr() << std::endl;
    std::cout<< "Max: " << max_.toStr() << std::endl;
    std::cout<< std::endl;
}

template<class T, Size D>
Size Box<T,D>::numberOfDifferentCoords() const {
    Size res = 0;
    for(Size d = 0; d < D; d++) {
        if (Math::Util::notEqual(max_(d),min_(d))) {
            res++;
        }
    }
    return res;
}

} /* namespace Geometry */
} /* namespace SEMBA */
