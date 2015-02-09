/*
 * PMSMA.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMSMA_H_
#define PMSMA_H_

#include "PMPredefined.h"

class PMSMA : public PMPredefined {
public:
	PMSMA();
	virtual ~PMSMA();
	PMSMA(const unsigned int id_, const string name_);
	PMSMA&
	 operator=(const PMSMA &param);
	PMSMA&
	 operator=(const PMPredefined &param);
	bool
	 isSMA() const;
	void
	 printInfo() const;
};

#endif /* PMSMA_H_ */
