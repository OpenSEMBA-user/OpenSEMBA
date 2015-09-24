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
