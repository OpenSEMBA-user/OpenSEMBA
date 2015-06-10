/*
 * CellTri6.h
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#ifndef CELLTRI6_H_
#define CELLTRI6_H_

#include "CellTri.h"

template <int TRI_N>
class CellTri6 : public CellTri<TRI_N>, public Tri6 {
	static const uint np = (TRI_N+1) * (TRI_N+2) / 2;
	static const uint nfp = (TRI_N + 1);
	static const uint faces = 3;
	static const uint vertices = 3;
public:
	CellTri6(
	 const Tri6& base);
	virtual
	~CellTri6();
	CartesianVector<complex<double>,3>
	 getRadiatedField(
	  const CartesianVector<complex<double>,3> electric[np],
	  const CartesianVector<complex<double>,3> magnetic[np],
	  const double frequency,
	  const pair<double, double> direction) const;
};

#include "CellTri6.hpp"

#endif /* CELLTRI6_H_ */
