/*
 * MagnitudeGaussian.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#include "MagnitudeGaussian.h"

MagnitudeGaussian::MagnitudeGaussian() {
    delay_ = 0.0;
    spread_ = 0.0;
    freq_ = 0.0;
}

MagnitudeGaussian::~MagnitudeGaussian() {
    // TODO Auto-generated destructor stub
}

double
MagnitudeGaussian::evaluate(const double time) const {
    double expArg = (time - delay_) / (spread_ * M_SQRT2);
    return exp(-expArg * expArg) * cos(freq_ * M_PI * 2.0 * time);
}
