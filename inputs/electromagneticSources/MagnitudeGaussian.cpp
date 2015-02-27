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

MagnitudeGaussian::MagnitudeGaussian(
        const double spread,
        const double delay,
        const double freq) {
    spread_ = spread;
    delay_ = delay;
    freq_ = freq;
}

MagnitudeGaussian::~MagnitudeGaussian() {
    // TODO Auto-generated destructor stub
}

double
MagnitudeGaussian::evaluate(const double time) const {
    static const double pi = acos(-1.0);
    static const double sqrt2 = sqrt(2.0);
    double expArg = (time - delay_) / (spread_ * sqrt2);
    return exp(-expArg * expArg) * cos(freq_ * pi * 2.0 * time);
}

void
MagnitudeGaussian::printInfo() const {
    cout << " --- Magnitude Gaussian info --- " << endl;
    cout << "Delay: " << delay_ << endl;
    cout << "Spread: " << spread_ << endl;
    cout << "Frequency: " << freq_ << endl;
}

double MagnitudeGaussian::getDelay() const {
   return delay_;
}

double MagnitudeGaussian::getFreq() const {
   return freq_;
}

double MagnitudeGaussian::getSpread() const {
   return spread_;
}
