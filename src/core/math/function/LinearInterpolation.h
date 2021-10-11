

#pragma once

#include <map>
#include <string>
#include <vector>

#include "Function.h"

namespace SEMBA {
namespace Math {
namespace Function {

template<class S, class T>
class LinearInterpolation : public Function<S,T> {
public:
    LinearInterpolation();
    LinearInterpolation(const std::vector<std::pair<S,T>>& xy);
    LinearInterpolation(const std::string& file);
    virtual ~LinearInterpolation();

    SEMBA_MATH_FUNCTION_DEFINE_CLONE(LinearInterpolation)

    T operator()(const S& arg) const;
    bool operator==(const Base& arg) const;
private:
    std::map<S,T> value_;

    void initFromFile(const std::string& file);
};

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

#include "LinearInterpolation.hpp"

