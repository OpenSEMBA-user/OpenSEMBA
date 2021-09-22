

#pragma once

#include "Group.h"

namespace SEMBA {
namespace Group {

template<typename T>
class Printable : public virtual Base<T> {
public:
    Printable() {}
    virtual ~Printable() {}

    void printInfo() const;

};

} /* namespace Group */
} /* namespace SEMBA */

#include "Printable.hpp"

