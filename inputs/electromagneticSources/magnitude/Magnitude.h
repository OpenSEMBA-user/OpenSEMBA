/*
 * Magnitude.h
 *
 *  Created on: Dec 13, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_MAGNITUDE_H_
#define SRC_COMMON_INPUTS_MAGNITUDE_H_

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include "Types.h"

#include "../../../base/ClassBase.h"

class Magnitude : public virtual ClassBase {
public:
    Magnitude();
    virtual ~Magnitude();

    virtual Real evaluate(const Real time) const = 0;

    virtual void printInfo() const = 0;

};

#endif /* SRC_COMMON_INPUTS_MAGNITUDE_H_ */
