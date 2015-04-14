/*
 * CellTri3.h
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#ifndef CELLTRI3_H_
#define CELLTRI3_H_

#include "CellTri.h"

template<int TRI_N>
class CellTri3 : public CellTri<TRI_N>, public Tri3 {
public:
	static const uint np = (TRI_N+1) * (TRI_N+2) / 2;
	static const uint nfp = (TRI_N + 1);
	static const uint faces = 3;
	static const uint vertices = 3;
	CellTri3(
	 const Tri3& base);
	virtual
	~CellTri3();
	CartesianVector<complex<double>,3>
	 getRadiatedField(
	  const CartesianVector<complex<double>,3> electric[np],
	  const CartesianVector<complex<double>,3> magnetic[np],
	  const double frequency,
	  const pair<double, double> direction) const;
};

#include "CellTri3.hpp"

#endif /* CELLTRI3_H_ */
