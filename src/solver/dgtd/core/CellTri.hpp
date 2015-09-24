/*
 * CellTri.cpp
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#include "CellTri.h"

template<Int N, class E>
CellTri<N,E>::CellTri() {
	// TODO Auto-generated constructor stub

}

template<Int N, class E>
CellTri<N,E>::~CellTri() {
	// TODO Auto-generated destructor stub
}

template<Int N, class E>
inline CVecC3 CellTri<N,E>::getRadiatedField(
 const CVecC3 electric[np],
 const CVecC3 magnetic[np],
 const Real frequency,
 const pair<Real, Real> direction) const {
    // Computes phase contribution.
    complex<Real> phase[geo.ncp];
    const Real beta = 2.0 * M_PI * frequency / Constants::c0;
    complex<Real> phaseShift(0.0, beta);
    SphericalVector sphDir(direction.first, direction.second);
    CVecR3 dir = sphDir.convertToCartesian();
    CVecR3 cNode[geo.ncp];
    getCubatureNodes(cNode);
    for (UInt c = 0; c < geo.ncp; c++) {
        phase[c] = exp(phaseShift * (Real) dir.dot(cNode[c]));
    }
    // Computes integral.
    const Real c0mu0 = Constants::c0 * Constants::mu0;
    CVecR3 cNormal[geo.ncp];
    getCubatureNormals(cNormal);
    Real csdf[geo.ncp];
    getCubatureDifferentials(csdf);
    CVecC3 res;
    CVecC3 complexDir;
    complexDir = MathUtils::convertToComplex(dir);
    CVecC3   complexNormal[geo.ncp];
    MathUtils::convertToComplex(complexNormal,cNormal,geo.ncp);
    static const SimplexTri<N> tri;
    CVecC3 M, J, integrand;
    for (UInt j = 0; j < tri.np; j++) {
        for (UInt c = 0; c < geo.ncp; c++) {
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
