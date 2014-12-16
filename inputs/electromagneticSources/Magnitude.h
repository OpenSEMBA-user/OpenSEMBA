/*
 * Magnitude.h
 *
 *  Created on: Dec 13, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_MAGNITUDE_H_
#define SRC_COMMON_INPUTS_MAGNITUDE_H_

#include <string>

using namespace std;

class Magnitude {
public:
    Magnitude();
    virtual ~Magnitude();
    virtual double
     evaluate(const double time) const = 0;
    virtual void
     printInfo() const;
private:
};

#endif /* SRC_COMMON_INPUTS_MAGNITUDE_H_ */
