/*
 * SolverWaveport.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: luis
 */

#include "DGWaveport.h"

DGWaveport::DGWaveport() {
	posSF = NULL;
	posTFNB = NULL;
	posTF = NULL;
	gauss = NULL;
	hm = NULL;
}

DGWaveport::~DGWaveport() {
	// TODO Auto-generated destructor stub
}
//
//Real
//DGWaveport::getNumericalGammaMGauss(
// const Real t,
// const Real dt,
// const Real amp,
// const Real delay,
// const Real spread,
// const Real kcm) const {
//	// Computes current step.
//	const UInt n = t / dt;
//	// Performs convolution.
//	UInt j;
//	Real res = 0.0;
//	for (j = 0; j < n; j++) {
//		res +=
//		 getHm(j*dt, kcm)
//		 * getGauss((n-j)*dt, amp,delay,spread);
//	}
//	res *= dt;
//	res += getGaussDerivative(t, amp,delay,spread) / c0;
//	return res;
//}

Real DGWaveport::getHm(
 const Real t,
 const Real kcm) const {
	if (t == 0) {
		return (kcm * kcm * Constants::c0 / 2.0);
	} else {
		return (kcm / t * j1(kcm * Constants::c0 * t));
	}
}

bool DGWaveport::checkNormalsAreEqual(
 const vector<pair<UInt, UInt> >& elemFace) const {
//	CVecR3 n1, n2;
//	for (UInt i = 1; i < elemFace.size(); i++) {
//		const UInt id1 = cells.getIdOfRelPos(elemFace[i-1].first);
//		const UInt f1 = elemFace[i-1].second;
//		n1 = cells.getPtrToCellWithId(id1)->getSideNormal(f1);
//		const UInt id2 = cells.getIdOfRelPos(elemFace[i].first);
//		const UInt f2 = elemFace[i].second;
//		n2 = cells.getPtrToCellWithId(id2)->getSideNormal(f2);
//		if (n1 != n2) {
//			return false;
//		}
//	}
//	return true;
}
