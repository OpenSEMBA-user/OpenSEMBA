/*
 * SolverWaveport.h
 *
 *  Created on: Aug 26, 2013
 *      Author: luis
 */

#ifndef SOLVERWAVEPORT_H_
#define SOLVERWAVEPORT_H_

#include "DGSource.h"

class DGWaveport : public DGSource {
public:
	DGWaveport();
	virtual
	~DGWaveport();
protected:
//	Real
//     getNumericalGammaMGauss(
//      const Real time,
//      const Real minDT,
//      const Real amplitude,
//      const Real delay,
//      const Real spread,
//      const Real kcm) const;
	bool
	 checkNormalsAreEqual(
	  const vector<pair<UInt,UInt> >& elemFace,
	  const CellGroup& cells) const;
protected:
	CVecR3* posTF;
	CVecR3* posTFNB;
	CVecR3* posSF;
private:
	Real *gauss, *hm;
	Real
	 getHm(
	  const Real time,
	  const Real kcm) const;
};

#endif /* SOLVERWAVEPORT_H_ */
