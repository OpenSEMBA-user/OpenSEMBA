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
//double
//DGWaveport::getNumericalGammaMGauss(
// const double t,
// const double dt,
// const double amp,
// const double delay,
// const double spread,
// const double kcm) const {
//	// Computes current step.
//	const uint n = t / dt;
//	// Performs convolution.
//	uint j;
//	double res = 0.0;
//	for (j = 0; j < n; j++) {
//		res +=
//		 getHm(j*dt, kcm)
//		 * getGauss((n-j)*dt, amp,delay,spread);
//	}
//	res *= dt;
//	res += getGaussDerivative(t, amp,delay,spread) / c0;
//	return res;
//}

double
DGWaveport::getHm(
 const double t,
 const double kcm) const {
	if (t == 0) {
		return (kcm * kcm * SPEED_OF_LIGHT / 2.0);
	} else {
		return (kcm / t * j1(kcm * SPEED_OF_LIGHT * t));
	}
}

bool
DGWaveport::checkNormalsAreEqual(
 const vector<pair<uint, uint> >& elemFace,
 const CellGroup& cells) const {
	CVecD3 n1, n2;
	for (uint i = 1; i < elemFace.size(); i++) {
		const uint id1 = cells.getIdOfRelPos(elemFace[i-1].first);
		const uint f1 = elemFace[i-1].second;
		n1 = cells.getPtrToCellWithId(id1)->getSideNormal(f1);
		const uint id2 = cells.getIdOfRelPos(elemFace[i].first);
		const uint f2 = elemFace[i].second;
		n2 = cells.getPtrToCellWithId(id2)->getSideNormal(f2);
		if (n1 != n2) {
			return false;
		}
	}
	return true;
}
