/*
 * GroupEMSource.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_
#define COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_

#include "EMSource.h"
#include "Dipole.h"
#include "Generator.h"
#include "PlaneWave.h"
#include "SourceOnLine.h"
#include "Waveport.h"

template<typename E = EMSourceBase>
class GroupEMSources : public virtual Group<E> {
public:
    USE_GROUP_CONSTRUCTS(GroupEMSources, E);

    DEFINE_GROUP_CLONE(GroupEMSources, E);

    USE_GROUP_ASSIGN(E);

    void printInfo() const;
};

#include "GroupEMSources.hpp"

#endif /* COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_ */
