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

class EMSource : public Condition,  public ClassBase {
public:
    EMSource();
    EMSource(
            const vector<ElementId>& elem,
            const Magnitude* magnitude);
    virtual ~EMSource();
    vector<ElementId> getElem() const;
    const Magnitude* getMagnitude() const;
    Condition::Type getConditionType() const;
    void printInfo() const;
private:
    vector<ElementId> elem_;
    const Magnitude* magnitude_;
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
