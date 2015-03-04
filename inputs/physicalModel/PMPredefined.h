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
    PMPredefined(const uint id, const string name);
	virtual ~PMPredefined();
	virtual void printInfo() const;
};

#endif /* PMPREDEFINED_H_ */
