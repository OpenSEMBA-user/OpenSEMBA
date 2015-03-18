/*
 * MagnitudeNumerical.h
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_

#include "Magnitude.h"
#include "../../ProjectFile.h"
#include <map>
#include <cmath>
#include <assert.h>

class MagnitudeNumerical: public Magnitude, public ProjectFile {
public:
    MagnitudeNumerical();
    virtual ~MagnitudeNumerical();
    MagnitudeNumerical(
            const string& filename);
    MagnitudeNumerical(
            const string& filename,
            const Magnitude* mag,
            const Real timeStep,
            const Real finalTime);
    MagnitudeNumerical& operator=(const MagnitudeNumerical& rhs);
    virtual Real evaluate(const Real time) const;
    virtual void printInfo() const;
private:
    map<Real,Real> value_;
    static const UInt defaultNumberOfSteps = 1e4;
    void initFromFile();
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_ */
