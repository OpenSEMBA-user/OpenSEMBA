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

template<int TRI_N>
class CellTri : public Cell {
public:
	static const SimplexTri<TRI_N> tri;
	static const uint np = (TRI_N+1) * (TRI_N+2) / 2;
	static const uint nfp = (TRI_N + 1);
	static const uint faces = 3;
	static const uint vertices = 3;
	CellTri();
	virtual
	~CellTri();
	virtual CartesianVector<complex<double>,3>
	 getRadiatedField(
	  const CartesianVector<complex<double>,3> electric[np],
	  const CartesianVector<complex<double>,3> magnetic[np],
	  const double frequency,
	  const pair<double,double> direction) const = 0;
	virtual uint
	 getNumberOfVertices() const;
	virtual CVecR3
	 getSideNormal(const uint s) const;
};

#include "CellTri.hpp"

#endif /* CELLTRI_H_ */
