#pragma once

namespace SEMBA {
namespace Class {

class Cloneable {
public:
    Cloneable() {}
    virtual ~Cloneable() {}


    virtual Cloneable* clone  () const = 0;
    template<typename T>
    T*                 cloneTo() const {
        return &dynamic_cast<T&>(*clone());
    }
};

} /* namespace Class */
} /* namespace SEMBA */

#ifndef SEMBA_CLASS_DEFINE_CLONE
#define SEMBA_CLASS_DEFINE_CLONE(NAME)  \
    NAME* clone() const {               \
        return new NAME(*this);         \
    }
#endif

