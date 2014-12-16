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

class MagnitudeNumerical: public Magnitude {
public:
    MagnitudeNumerical();
    virtual ~MagnitudeNumerical();
    MagnitudeNumerical(const Magnitude& mag);
    virtual double
     evaluate(const double time) const;
    const string& getFilename() const;
private:
    string filename_;
    map<double,double> value_;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_ */
