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

template<typename G>
class ClassGroupBase : public virtual ClassBase {
public:
    ClassGroupBase();
    virtual ~ClassGroupBase();

    virtual G elems() const = 0;

    virtual void set(const G&) = 0;
    virtual void add(const G&) = 0;
};

#include "ClassGroupBase.hpp"

#endif /* SRC_COMMON_BASE_CLASSGROUPBASE_H_ */
