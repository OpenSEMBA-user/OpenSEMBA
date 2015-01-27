/*
 * PMPMC.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMPMC_H_
#define PMPMC_H_

#include "PMPredefined.h"

class PMPMC : public PMPredefined {
public:
	PMPMC();
	virtual ~PMPMC();
	PMPMC(const unsigned int id, const string name);
	PMPMC&
	 operator=(const PMPMC &param);
	PMPMC&
	 operator=(const PMPredefined &param);
	bool
	 isPMC() const;
   Condition::Type
    getConditionType() const;
	void
	 printInfo() const;
};


#endif /* PMPMC_H_ */
