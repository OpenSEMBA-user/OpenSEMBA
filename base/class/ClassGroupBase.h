/*
 * ClassGroup.h
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_BASE_CLASSGROUPBASE_H_
#define SRC_COMMON_BASE_CLASSGROUPBASE_H_

#include "Group.h"

#include "ClassBase.h"

template<typename T>
class ClassGroupBase : public virtual ClassBase {
public:
    ClassGroupBase();
    virtual ~ClassGroupBase();

    virtual Group<const T> elems() const = 0;

    virtual void set(const Group<const T>&) = 0;
    virtual void add(const Group<const T>&) = 0;
};

#include "ClassGroupBase.hpp"

#endif /* SRC_COMMON_BASE_CLASSGROUPBASE_H_ */
