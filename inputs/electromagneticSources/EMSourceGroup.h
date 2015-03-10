/*
 * EMSourceGroup.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef EMSOURCEGROUP_H_
#define EMSOURCEGROUP_H_

#include "EMSource.h"
#include "Dipole.h"
#include "Generator.h"
#include "PlaneWave.h"
#include "SourceOnLine.h"
#include "Waveport.h"

template<typename C = EMSource>
class EMSourceGroup : public Group<EMSource> {
public:
	EMSourceGroup();
	EMSourceGroup& operator=(const EMSourceGroup &rhs);
	EMSourceGroup<C>& operator=(const Group<C>& rhs);
	UInt countWithType(const Element::Type type) const;
	const EMSource* getWithType(const UInt i, const Element::Type type) const;
	void printInfo() const;
};

#include "EMSourceGroup.hpp"

#endif /* EMSOURCEGROUP_H_ */
