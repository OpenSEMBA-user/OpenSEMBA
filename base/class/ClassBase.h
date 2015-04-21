#ifndef CLASSBASE_H_
#define CLASSBASE_H_

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
using namespace std;

class ClassBase {
public:
    ClassBase() {}
    virtual ~ClassBase() {}

    template<typename T>
    bool is() const;

    virtual ClassBase* clone() const = 0;

    template<typename T>
    typename add_pointer<typename remove_const<T>::type>::type cloneTo() const;

    template<typename T>
    T* castTo();
    template<typename T>
    const T* castTo() const;

    virtual void printInfo() const = 0;
};

#include "ClassBase.hpp"

#define DEFINE_CLONE(NAME)      \
    ClassBase* clone() const {  \
        return new NAME(*this); \
    }

#endif /* CLASSBASE_H_ */
