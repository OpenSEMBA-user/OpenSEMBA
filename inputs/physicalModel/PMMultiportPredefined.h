/*
 * PMMultiportPredefined.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMMULTIPORTPREDEFINED_H_
#define PMMULTIPORTPREDEFINED_H_

#include "PMMultiport.h"

class PMMultiportPredefined: public PMMultiport {
public:
	PMMultiportPredefined(
	 const MatId idIn,
	 const string nameIn,
	 const PMMultiport::Type);
	virtual ~PMMultiportPredefined();

    virtual ClassBase* clone() const;

	virtual void printInfo() const;
};

#endif /* PMMULTIPORTPREDEFINED_H_ */
