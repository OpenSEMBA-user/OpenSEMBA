/*
 * PMSurface.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMSURFACE_H_
#define PMSURFACE_H_

#include "PhysicalModel.h"

class PMSurface : public PhysicalModel {
	friend class SolverSIBC;
public:
	PMSurface();
	PMSurface(
	 const unsigned int id_, const string& name_,
	 const double Zinfinite_[4], const double Zstatic_[4],
	 const vector<double>& pole_,
	 const vector<double>& Z11_, const vector<double>& Z12_,
	 const vector<double>& Z21_, const vector<double>& Z22_);
	virtual ~PMSurface();
	PMSurface&
     operator=(const PMSurface &param);
	void
	 modifyZinfinite(const double Zinf_[4]);
	void
	 modifyZstatic(const double Zsta_[4]);
	void
	 addPole(
	  const double pole_,
	  const double Z11_, const double Z12_,
      const double Z21_, const double Z22_);
	bool
	 isSurfaceImpedance() const {return true;}
	unsigned int
	 getNumberOfPoles() const {
		 return pole.size();
	 }
	bool
	 isSurface() const {return true;}
	void
	 printInfo() const;
protected:
	// All the impedances are stored in this way: [ Z[0] Z[1] ]
	//                                            [ Z[2] Z[3] ]
	double Zinfinite[4]; // Impedances @ infte. freq.
	double Zstatic[4]; // Impedances @ zero freq.
	vector<double> pole; // Impedance poles.
	vector<double> Z11, Z12, Z21, Z22; // Impedance residuals.
};

#endif /* PMSURFACE_H_ */
