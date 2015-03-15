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

template<typename E = EMSource>
class EMSourceGroup : public Group<E> {
public:
    EMSourceGroup();
    EMSourceGroup(const vector<E*>&);
    EMSourceGroup(const Group<E>& rhs);
    virtual ~EMSourceGroup();

    EMSourceGroup<E>& operator=(const Group<E>& rhs);

    void printInfo() const;
};

#include "EMSourceGroup.hpp"

#endif /* EMSOURCEGROUP_H_ */
