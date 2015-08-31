/*
 * ClassGroup.h
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_BASE_CLASSGROUPBASE_H_
#define SRC_COMMON_BASE_CLASSGROUPBASE_H_

#include <string>

#include "base/group/Group.h"

#include "ClassBase.h"

template<typename G>
class ClassGroupBase : public virtual ClassBase {
public:
    class ErrorEmpty : public Error {
    public:
        ErrorEmpty(const std::string&);
        ~ErrorEmpty() throw();
    };

    ClassGroupBase();
    virtual ~ClassGroupBase();

    bool check(const std::string& = "Group") const;

    virtual G elems() const = 0;

    virtual void set(const G&) = 0;
    virtual void add(const G&) = 0;
};

#include "ClassGroupBase.hpp"

#endif /* SRC_COMMON_BASE_CLASSGROUPBASE_H_ */
