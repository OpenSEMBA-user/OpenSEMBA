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

template<typename E = EMSource<> >
class EMSourceGroup : public Group<E> {
public:
    EMSourceGroup();
    template<typename E2>
    EMSourceGroup(const vector<E2*>&);
    template<typename E2>
    EMSourceGroup(const Group<E2>& rhs);
    virtual ~EMSourceGroup();

    template<typename E2>
    EMSourceGroup<E>& operator=(const Group<E2>& rhs);

    void printInfo() const;
};

#include "EMSourceGroup.hpp"

#endif /* EMSOURCEGROUP_H_ */
