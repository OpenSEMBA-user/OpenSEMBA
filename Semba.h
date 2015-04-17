/*
 * Semba.h
 *
 *  Created on: Apr 14, 2015
 *      Author: luis
 */

#ifndef SEMBA_H_
#define SEMBA_H_

#include "Arguments.h"
#include "Solver.h"

class Semba {
public:
    Semba(const Arguments& args);
    virtual ~Semba();

    void run();
private:
    Solver* solver_;
};

#endif /* SRC_APPS_SEMBA_SEMBA_H_ */
