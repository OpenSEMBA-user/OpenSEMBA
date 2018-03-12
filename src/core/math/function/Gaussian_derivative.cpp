/*
 * Gaussian_derivative.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: alejandra
 */
#include <cmath>

#include "Gaussian_derivative.h"

namespace SEMBA{
namespace Math{
namespace Function{

GaussDerivative::GaussDerivative()
					: Gaussian(){

}

GaussDerivative::GaussDerivative(const Real spread,
									   const Real delay,
									   const Real freq)
	: Gaussian(spread, delay, freq){

}

GaussDerivative::GaussDerivative(const Gaussian& rhs)
: Gaussian(rhs){

}

GaussDerivative::~GaussDerivative(){

}

Real GaussDerivative::operator ()(const Real& time) const {
    static const Real pi    = Constants::pi;
    static const Real sqrt2 = sqrt(2.0);
    Real expArg = (time - delay_) / (spread_ * sqrt2);
    Real factor = -(time - delay_) / pow(spread_, 2);

    return factor * exp(-expArg * expArg) * cos(freq_ * pi * 2.0 * time);
}


}/*namespace Function*/
}/*namespace Math*/
}/*namespace SEMBA*/







