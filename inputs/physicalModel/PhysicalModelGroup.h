/*
 * PhysicalModelGroup.h
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef PHYSICALMODELGROUP_H_
#define PHYSICALMODELGROUP_H_

#include <math.h>
#include <cmath>
#include <vector>
#include <assert.h>
#include "GroupId.h"
#include "PhysicalModel.h"
#include "PMPEC.h"
#include "PMPMC.h"
#include "PMSMA.h"
#include "PMVolumeDispersive.h"
#include "PMVolumePML.h"
#include "PMMultiportPredefined.h"
#include "PMMultiportRLC.h"
#include "PMSurfaceSIBC.h"
#include "PMSurfaceMultilayer.h"
#include "PMWire.h"

template<typename P = PhysicalModel>
class PhysicalModelGroup : public GroupId<P,MatId> {
public:
    PhysicalModelGroup();
    PhysicalModelGroup(const vector<P*>&);
    PhysicalModelGroup(const Group<P>& rhs);
    virtual ~PhysicalModelGroup();

    PhysicalModelGroup<P>& operator=(const Group<P>& rhs);

    vector<MatId> getIds() const;
    void printInfo() const;
private:
    void getDirection(
            PMVolumePML::Direction direction[3],
            const UInt i) const;
    PMVolumePML::Direction getDirectionFromInt(const UInt i) const;
};

#include "PhysicalModelGroup.hpp"

#endif /* PHYSICALMODELGROUP_H_ */
