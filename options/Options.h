/*
 * Options.h
 *
 *  Created on: Mar 10, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OPTIONS_H_
#define SRC_COMMON_INPUTS_OPTIONS_H_

#include "ClassBase.h"
#include "Arguments.h"

class Options : public virtual ClassBase {
public:
    virtual ~Options();

    virtual void set(Arguments& args) = 0;

    virtual void printHelp() const = 0;
};

#endif /* SRC_COMMON_INPUTS_OPTIONS_H_ */
