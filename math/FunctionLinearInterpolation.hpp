/*
 * FunctionLinearInterpolation.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#include "FunctionLinearInterpolation.h"

template<class S, class T>
inline FunctionLinearInterpolation<S, T>::FunctionLinearInterpolation() {
}

template<class S, class T>
inline FunctionLinearInterpolation<S, T>::FunctionLinearInterpolation(
        const vector<pair<S, T> >& xy) {
    for (UInt i = 0; i < xy.size(); i++) {
        value_.insert(xy[i]);
    }
}

template<class S, class T>
inline FunctionLinearInterpolation<S, T>::FunctionLinearInterpolation(
        const ProjectFile& file) {
    initFromFile(file);
}

template<class S, class T>
inline FunctionLinearInterpolation<S, T>::~FunctionLinearInterpolation() {
}

template<class S, class T>
inline void FunctionLinearInterpolation<S, T>::printInfo() const {
    Function<S,T>::printInfo();
}

template<class S, class T>
inline T FunctionLinearInterpolation<S, T>::operator ()(const S& pos) const {
    if (value_.empty()) {
        throw Error("Attempting to interpolate with no data.");
    }
    pair<typename map<S,T>::const_iterator, typename map<S,T>::const_iterator> range;
    range = value_.equal_range(pos);
    if (range.first == range.second) {
        if (range.first == value_.end()) {
            return value_.rbegin()->second;
        } else {
            if (pos <= range.first->first && range.first == value_.begin()) {
                return range.first->second;
            } else {
                typename map<S,T>::const_iterator prev = range.first;
                --prev;
                const Real x1 = prev->first;
                const Real y1 = prev->second;
                const Real x2 = range.second->first;
                const Real y2 = range.second->second;
                const Real m = (y2 - y1) / (x2 - x1);
                const Real n = (x2*y1 - x1*y2)/(x2 - x1);
                return (m * pos + n);
            }
        }
    } else {
        if (range.first->first == pos) {
            return range.first->second;
        } else {
            return range.first->second;
        }
    }
}

template<class S, class T>
inline bool FunctionLinearInterpolation<S, T>::operator ==(
        const FunctionBase& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    const FunctionLinearInterpolation<S,T>* rhsPtr =
            rhs.castTo<FunctionLinearInterpolation<S,T>>();
    return value_ == rhsPtr->value_;
}

template<class S, class T>
void FunctionLinearInterpolation<S, T>::initFromFile(
        const ProjectFile& file) {
    static_assert(std::is_same<S, Real>::value, "S must be Real");
    static_assert(std::is_same<T, Real>::value, "T must be Real");
    ifstream iStream;
    iStream.open(file.getFilename().c_str(), ifstream::in);
    if (iStream.fail()) {
        throw ErrorFileNotExists(file.getFilename());
    }

    while (!iStream.eof()) {
        pair<Real, Real> value;
        iStream >> value.first >> value.second;
        value_.insert(value);
    }

    if (value_.size() == 0) {
        throw ErrorFileEmpty(file.getFilename());
    }
}
