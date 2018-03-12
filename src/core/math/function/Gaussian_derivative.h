/*
 * Gaussian_derivative.h
 *
 *  Created on: Mar 12, 2018
 *      Author: alejandra
 */


#include "Gaussian.h"

namespace SEMBA{
namespace Math{
namespace Function{

class GaussDerivative : public Gaussian{
public:
	GaussDerivative();
	GaussDerivative(const Real spread,
					   const Real delay,
					   const Real freq);
	GaussDerivative(const Gaussian& rhs);

	virtual ~GaussDerivative();

	virtual Real operator()(const Real&) const;
};

}/*namespace Function*/
}/*namespace Math*/
}/*namespace SEMBA*/





