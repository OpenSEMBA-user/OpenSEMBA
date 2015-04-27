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

class ClassBase : public enable_shared_from_this<ClassBase> {
public:
    ClassBase() {}
    virtual ~ClassBase() {}

    template<typename T>
    bool is() const;

    virtual ClassBase* clone() const = 0;

    template<typename T>
    typename add_pointer<typename remove_const<T>::type>::type cloneTo() const;
    template<typename T>
    shared_ptr<typename remove_const<T>::type> cloneToSharedPtr() const;

    shared_ptr<ClassBase>       getSharedPtr();
    shared_ptr<const ClassBase> getSharedPtr() const;

    template<typename T>
    T* castTo();
    template<typename T>
    const T* castTo() const;

    template<typename T>
    shared_ptr<T> castToSharedPtr();
    template<typename T>
    shared_ptr<const T> castToSharedPtr() const;

    virtual void printInfo() const = 0;
};

#include "ClassBase.hpp"

#define DEFINE_CLONE(NAME)      \
    ClassBase* clone() const {  \
        return new NAME(*this); \
    }

#endif /* CLASSBASE_H_ */
