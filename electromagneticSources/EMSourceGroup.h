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

template<typename E = EMSourceBase>
class EMSourceGroup : public virtual Group<E> {
public:
    USE_GROUP_CONSTRUCTS(EMSourceGroup, E);

    DEFINE_GROUP_CLONE(EMSourceGroup, E);

    USE_GROUP_ASSIGN(E);

    void printInfo() const;
};

#include "EMSourceGroup.hpp"

#endif /* EMSOURCEGROUP_H_ */
