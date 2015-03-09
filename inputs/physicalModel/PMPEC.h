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
	PMPEC(const MatId id, const string name);
	virtual ~PMPEC();

    ClassBase* clone() const;

	void printInfo() const;
};

#endif /* PMPEC_H_ */
