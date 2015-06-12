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
//	double
//     getNumericalGammaMGauss(
//      const double time,
//      const double minDT,
//      const double amplitude,
//      const double delay,
//      const double spread,
//      const double kcm) const;
	bool
	 checkNormalsAreEqual(
	  const vector<pair<uint,uint> >& elemFace,
	  const CellGroup& cells) const;
protected:
	CVecR3* posTF;
	CVecR3* posTFNB;
	CVecR3* posSF;
private:
	double *gauss, *hm;
	double
	 getHm(
	  const double time,
	  const double kcm) const;
};

#endif /* SOLVERWAVEPORT_H_ */
