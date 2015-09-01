/*
 * PMVolume.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMVOLUME_H_
#define PMVOLUME_H_

#include "PhysicalModel.h"

class PMVolume : public PhysicalModel {
public:
	PMVolume(const MatId id, const string name);
	virtual ~PMVolume() = 0;

    virtual void printInfo() const;
};

#endif /* PMVOLUME_H_ */
