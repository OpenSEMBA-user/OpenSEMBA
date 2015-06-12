/*
 * GroupEMSource.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_
#define COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_

#include "../sources/Dipole.h"
#include "../sources/EMSource.h"
#include "../sources/Generator.h"
#include "../sources/PlaneWave.h"
#include "../sources/SourceOnLine.h"
#include "../sources/WaveportRectangular.h"

template<typename E = EMSourceBase>
class GroupEMSources : public virtual Group<E> {
public:
    USE_GROUP_CONSTRUCTS(GroupEMSources, E);

    DEFINE_GROUP_CLONE(GroupEMSources, E);

    USE_GROUP_ASSIGN(E);

    bool isSimilar(const GroupEMSources& rhs) const;
    bool check() const;

    void printInfo() const;
};

#include "../sources/GroupEMSources.hpp"

#endif /* COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_ */
