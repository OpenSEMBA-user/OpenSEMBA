/*
 * MagnitudeNumerical.h
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_

#include "Magnitude.h"
#include <map>
#include <cmath>
#include <assert.h>

class MagnitudeNumerical: public Magnitude {
public:
    MagnitudeNumerical();
    virtual ~MagnitudeNumerical();
    MagnitudeNumerical(
            const string& filename);
    MagnitudeNumerical(
            const Magnitude* mag,
            const double timeStep,
            const double finalTime);
    virtual double
     evaluate(const double time) const;
    const string& getFilename() const;
    virtual void
     printInfo() const;
private:
    string filename_;
    map<double,double> value_;
    static const unsigned int defaultNumberOfSteps = 1e4;
    void initFromFile(const string& filename);
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_ */
