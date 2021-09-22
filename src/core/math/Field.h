

#pragma once

#include <cstdlib>

#include "math/vector/Cartesian.h"

namespace SEMBA {
namespace Math {

template <class T, std::size_t D, std::size_t N>
class Field {
public:
    Field();
    virtual ~Field();

    T*       operator()(const std::size_t i);
    const T* operator()(const std::size_t i) const;

    Vector::Cartesian<T,D> getCVec(const std::size_t i) const;

private:
    array<T, D*N> val_;
};

} /* namespace Math */
} /* namespace SEMBA */

#include "Field.hpp"

