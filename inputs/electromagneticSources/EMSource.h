// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include "../Condition.h"
#include "Magnitude.h"
#include "MagnitudeGaussian.h"
#include "../../math/CartesianVector.h"
#include "../../math/Constants.h"

using namespace std;

class EMSource: public Condition,  public ClassBase{
public:
    EMSource();
    EMSource(const Magnitude* magnitude);
    virtual ~EMSource();
    const Magnitude* getMagnitude() const;
    Condition::Type getConditionType() const;
    void printInfo() const;
    void convertToNumerical(
            const string file,
            const double step,
            const double finalTime);
private:
    const Magnitude* magnitude_;
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
