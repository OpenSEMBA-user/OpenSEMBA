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
	virtual ~PMPMC();
	PMPMC(const unsigned int id, const string name);

    ClassBase* clone() const;

	void printInfo() const;
};


#endif /* PMPMC_H_ */
