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
	PMPEC();
	virtual ~PMPEC();
	PMPEC(const unsigned int id, const string name);
	PMPEC&
	 operator=(const PMPEC &param);
	PMPEC&
	 operator=(const PMPredefined &param);
	bool
	 isPEC() const;
	Condition::Type
	 getConditionType() const;
	void
	 printInfo() const;
};

#endif /* PMPEC_H_ */
