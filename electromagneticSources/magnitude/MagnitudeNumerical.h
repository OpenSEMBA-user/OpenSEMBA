/*
 * MagnitudeNumerical.h
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_

#include <cassert>
#include <cmath>
#include <map>
using namespace std;

#include "ProjectFile.h"

#include "Magnitude.h"

class MagnitudeNumerical : public virtual Magnitude,
                           public virtual ProjectFile {
public:
    MagnitudeNumerical();
    MagnitudeNumerical(const string& filename);
    MagnitudeNumerical(const string& filename,
                       const Magnitude* mag,
                       const Real timeStep,
                       const Real finalTime);
    MagnitudeNumerical(const MagnitudeNumerical& rhs);
    virtual ~MagnitudeNumerical();

    DEFINE_CLONE(MagnitudeNumerical);

    MagnitudeNumerical& operator=(const MagnitudeNumerical& rhs);

    bool operator==(const Magnitude&) const;

    Real evaluate(const Real time) const;

    void printInfo() const;

private:
    static const UInt defaultNumberOfSteps = 1000;

    map<Real,Real> value_;

    void initFromFile();
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_ */
