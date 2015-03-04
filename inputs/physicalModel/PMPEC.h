/*
 * PMPEC.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMPEC_H_
#define PMPEC_H_

#include "PMPredefined.h"

class PMPEC : public PMPredefined {
public:
	PMPEC(const uint id, const string name);
	virtual ~PMPEC();
	bool isPEC() const;
	Condition::Type getConditionType() const;
	void printInfo() const;
};

#endif /* PMPEC_H_ */
