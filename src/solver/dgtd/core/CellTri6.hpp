/*
 * CellTri6.cpp
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#include "CellTri6.h"

template<int TRI_N>
CellTri6<TRI_N>::CellTri6(
 const Tri6& base) {
	assert(this->tri.ncp == geo.ncp);
	id = base.getId();
	normal = base.getNormal();
	for (uint i = 0; i < geo.np; i++) {
		v[i] = base.getV(i);
	}
}

template<int TRI_N>
CellTri6<TRI_N>::~CellTri6() {
	// TODO Auto-generated destructor stub
}

template<int TRI_N>
inline CartesianVector<complex<double>,3>
CellTri6<TRI_N>::getRadiatedField(
 const CartesianVector<complex<double>, 3> electric[np],
 const CartesianVector<complex<double>, 3> magnetic[np],
 const double frequency,
 const pair<double, double> direction) const {
	// Computes phase contribution.
	complex<double> phase[geo.ncp];
	const double beta = 2.0 * M_PI * frequency / SPEED_OF_LIGHT;
	complex<double> phaseShift(0.0, beta);
	SphericalVector sphDir(direction.first, direction.second);
	CVecD3 dir = sphDir.convertToCartesian();
	CVecD3 cNode[geo.ncp];
	getCubatureNodes(cNode);
	for (uint c = 0; c < geo.ncp; c++) {
		phase[c] = exp(phaseShift * (double) dir.dot(cNode[c]));
	}
	// Computes integral.
	const double c0mu0 = SPEED_OF_LIGHT * VACUUM_PERMEABILITY;
	CVecD3 cNormal[geo.ncp];
	getCubatureNormals(cNormal);
	double csdf[geo.ncp];
	getCubatureDifferentials(csdf);
	CartesianVector<complex<double>,3> res;
	CartesianVector<complex<double>,3> complexDir;
	complexDir = MathUtils::convertToComplex(dir);
	CartesianVector<complex<double>, 3>	 complexNormal[geo.ncp];
	MathUtils::convertToComplex(complexNormal,cNormal,geo.ncp);
	static const SimplexTri<TRI_N> tri;
	CartesianVector<complex<double>,3> M, J, integrand;
	for (uint j = 0; j < tri.np; j++) {
		for (uint c = 0; c < geo.ncp; c++) {
			M = - (complexNormal[c] ^ electric[j]);
			J =   (complexNormal[c] ^ magnetic[j]);
			integrand = ((J ^ complexDir) * c0mu0 + M) ^ complexDir;
			res += integrand *
			 (phase[c] * geo.cw[c] * csdf[c] / 2.0) * tri.ca[j][c];
		}
	}
	//res *= (phaseShift / (M_PI * 4.0)) * exp(phaseShift);
	return res;
}
