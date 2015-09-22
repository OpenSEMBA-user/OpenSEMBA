#ifndef CLASSBASE_H_
#define CLASSBASE_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

class ClassBase : public std::enable_shared_from_this<ClassBase> {
public:
    ClassBase();
    virtual ~ClassBase();

    template<typename T>
    bool is() const;

    virtual ClassBase* clone  () const = 0;
    template<typename T>
    T*                 cloneTo() const;

    std::shared_ptr<      ClassBase> getSharedPtr();
    std::shared_ptr<const ClassBase> getSharedPtr() const;

    template<typename T>
    T*       castTo();
    template<typename T>
    const T* castTo() const;

    virtual void printInfo() const = 0;
};

#include "ClassBase.hpp"

#define DEFINE_CLONE(NAME)      \
    NAME* clone() const {       \
        return new NAME(*this); \
    }

#endif /* CLASSBASE_H_ */
