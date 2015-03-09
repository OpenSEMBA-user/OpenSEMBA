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
	 const MatId id,
	 const string& name,
	 const MatD22& Zinfinite,
	 const MatD22& Zstatic,
	 const vector<double>& pole,
	 const vector<MatD22>& Z);
	virtual ~PMSurfaceSIBC();

    ClassBase* clone() const;

	virtual PMSurfaceSIBC&
     operator=(const PMSurfaceSIBC &param);
	void setZInfinity(const MatD22& Zinf);
	void setZStatic(const MatD22& Zsta);
	virtual void addPole(const double pole, const MatD22& Z);
    virtual uint getNumberOfPoles() const;
	virtual void printInfo() const;
protected:
	MatD22 ZInfinity_, ZStatic_;
	vector<double> pole_; // Impedance poles.
	vector<MatD22> Z_; // Impedance residuals.
};

#endif /* PMSURFACE_H_ */
