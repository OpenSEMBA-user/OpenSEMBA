/*
 * Magnitude.h
 *
 *  Created on: Dec 13, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_MAGNITUDE_H_
#define SRC_COMMON_INPUTS_MAGNITUDE_H_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "math/Function.h"
#include "base/class/ClassBase.h"

class Magnitude : public virtual ClassBase {
public:
    Magnitude();
    Magnitude(FunctionRR* mathFunction);
    virtual ~Magnitude();

    DEFINE_CLONE(Magnitude);

    Magnitude& operator=(const Magnitude& rhs);

    virtual bool operator==(const Magnitude&) const;

    Real evaluate(const Real time) const;

    virtual void printInfo() const;
private:
    FunctionRR* mathFunction_;
};

#endif /* SRC_COMMON_INPUTS_MAGNITUDE_H_ */
