/*
 * PMPredefined.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMPREDEFINED_H_
#define PMPREDEFINED_H_

#include "PhysicalModel.h"

class PMPredefined : public PhysicalModel {
public:
	PMPredefined() {};
	virtual ~PMPredefined() {};
	virtual PMPredefined&
	 operator=(const PMPredefined &param);
	bool
	 isPMPredefined() const {return true;}
	virtual bool
	 isSMA() const {return false;}
	virtual bool
	 isPEC() const {return false;}
	virtual bool
	 isPMC() const {return false;}
	virtual void
	 printInfo() const;
};

#endif /* PMPREDEFINED_H_ */
