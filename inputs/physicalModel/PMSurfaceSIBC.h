/*
 * PMSurface.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMSURFACESIBC_H_
#define PMSURFACESIBC_H_

#include "PMSurface.h"
#include "../../math/StaMatrix.h"

class PMSurfaceSIBC : public PMSurface {
	friend class DGSIBC;
public:
	PMSurfaceSIBC();
	PMSurfaceSIBC(
	 const unsigned int id, const string& name,
	 const StaMatrix<double,2,2>& Zinfinite,
	 const StaMatrix<double,2,2>& Zstatic,
	 const vector<double>& pole,
	 const vector<StaMatrix<double,2,2> >& Z);
	virtual ~PMSurfaceSIBC();
	PMSurfaceSIBC&
     operator=(const PMSurfaceSIBC &param);
	void
	 setZInfinity(const StaMatrix<double,2,2>& Zinf);
	void
	 setZStatic(const StaMatrix<double,2,2>& Zsta);
	void
	 addPole(
	  const double pole,
	  const StaMatrix<double,2,2>& Z);
	unsigned int
	 getNumberOfPoles() const {
		 return pole_.size();
	 }
	void
	 printInfo() const;
private:
	StaMatrix<double,2,2> ZInfinity_, ZStatic_;
	vector<double> pole_; // Impedance poles.
	vector<StaMatrix<double,2,2> > Z_; // Impedance residuals.
};

#endif /* PMSURFACE_H_ */
