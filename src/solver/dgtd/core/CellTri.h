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
 * CellTri.h
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#ifndef CELLTRI_H_
#define CELLTRI_H_

#include "Cell.h"
#include <complex>
#include "../../../common/math/Constants.h"
#include "../../../common/math/MathUtils.h"
#include "../../../common/math/SphericalVector.h"
#include "../../../common/geometry/elements/SurfR.h"

template<Int N, class E>
class CellTri : public Cell, public E {
public:
	static const SimplexTri<TRI_N> tri;
	static const UInt np = (TRI_N+1) * (TRI_N+2) / 2;
	static const UInt nfp = (TRI_N + 1);
	static const UInt faces = 3;
	static const UInt vertices = 3;
	CellTri();
	virtual
	~CellTri();
	virtual CVecC3 getRadiatedField(
	  const CVecC3 electric[np],
	  const CVecC3 magnetic[np],
	  const Real frequency,
	  const pair<Real,Real> direction) const = 0;
};

#include "CellTri.hpp"

#endif /* CELLTRI_H_ */
