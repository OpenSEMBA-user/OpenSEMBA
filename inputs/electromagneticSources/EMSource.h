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

class EMSource : public Condition {
public:
    EMSource();
    EMSource(
            const vector<uint>& elem,
            const Magnitude* magnitude);
    EMSource(
            const BoxD3& bound,
            const Magnitude* magnitude);
    virtual ~EMSource();
    vector<unsigned int> getElem() const;
    const BoxD3* getBound() const;
    void printInfo() const;
    const Magnitude* getMagnitude() const;
    Condition::Type getConditionType() const;
    virtual void applyGeometricScalingFactor(const double factor);
protected:
    BoxD3* bound_;
private:
    vector<unsigned int> elem_;
    const Magnitude* magnitude_;
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
