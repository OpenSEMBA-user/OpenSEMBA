

#pragma once

#include <type_traits>

#include "Group.h"

namespace SEMBA {
namespace Group {

template<typename T>
class Cloneable : public virtual Base<T> {
public:
    Cloneable() {}
    virtual ~Cloneable() {}

    Group<T>* clone() const;
    Group<typename std::remove_const<T>::type> cloneElems() const;
};

} /* namespace Group */
} /* namespace SEMBA */

#include "Cloneable.hpp"

#ifndef SEMBA_GROUP_DEFINE_CLONE
#define SEMBA_GROUP_DEFINE_CLONE(NAME, T)       \
    NAME<T>* clone() const {                    \
        return new NAME<T>(this->cloneElems()); \
    }
#endif

