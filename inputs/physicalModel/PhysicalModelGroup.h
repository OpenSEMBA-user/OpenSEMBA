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
#include "../../base/GroupId.h"
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
    template<typename P2>
    PhysicalModelGroup(const vector<P2*>&);
    PhysicalModelGroup(const Group<P>& rhs);
    template<typename P2>
    PhysicalModelGroup(const Group<P2>& rhs);
    virtual ~PhysicalModelGroup();

    PhysicalModelGroup<P>& operator=(const Group<P>& rhs);
    template<typename P2>
    PhysicalModelGroup<P>& operator=(const Group<P2>& rhs);

    vector<MatId> getIds() const;
    void printInfo() const;
private:
    void getDirection(PMVolumePML::Direction direction[3],
                      const UInt i) const;
    PMVolumePML::Direction getDirectionFromInt(const UInt i) const;
};

#include "PhysicalModelGroup.hpp"

#endif /* PHYSICALMODELGROUP_H_ */
