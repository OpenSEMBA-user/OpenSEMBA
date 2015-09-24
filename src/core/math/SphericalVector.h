// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * SphericalVector.h
 *
 *  Created on: Sep 1, 2012
 *      Author: luis
 */
#ifndef SPHERICALVECTOR_H_
#define SPHERICALVECTOR_H_

#include <cmath>
#include <iostream>
using namespace std;

#include "CartesianVector.h"

class SphericalVector {
public:
	Real radius;
	Real theta, phi;
	SphericalVector();
	SphericalVector(const Real theta, const Real phi);
	SphericalVector& operator=(const CVecR3& param);
	CVecR3 convertSphericalVectorField(
	  const Real ar,
	  const Real at,
	  const Real ap) const;
	CVecR3 convertToCartesian() const;
	Real norm() const;
	void printInfo() const;
};

#endif /* SPHERICALVECTOR_H_ */
