/*
 * ClassCompBase.h
 *
 *  Created on: 1/4/2015
 *      Author: Daniel
 */

#ifndef CLASSCOMPBASE_H_
#define CLASSCOMPBASE_H_

#include <typeinfo>

#include "ClassBase.h"

class ClassCompBase : public virtual ClassBase {
public:
    ClassCompBase() {}
    virtual ~ClassCompBase() {}

    bool isSameType(const ClassCompBase&) const;

    virtual bool isSimilar(const ClassCompBase& rhs) const;

    virtual bool operator==(const ClassCompBase&) const;
    virtual bool operator!=(const ClassCompBase&) const;
    virtual bool operator< (const ClassCompBase&) const;
    virtual bool operator<=(const ClassCompBase&) const;
    virtual bool operator> (const ClassCompBase&) const;
    virtual bool operator>=(const ClassCompBase&) const;
};

#endif /* CLASSCOMPBASE_H_ */
