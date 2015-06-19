/*
 * PhysicalModelGroup.h
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef COMMON_PHYSICALMODEL_GROUPPHYSICALMODELS_H_
#define COMMON_PHYSICALMODEL_GROUPPHYSICALMODELS_H_

#include <cassert>
#include <cmath>
#include <vector>
using namespace std;

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
#include "PMWireExtremes.h"

#include "base/group/GroupId.h"

template<typename P = PhysicalModel>
class GroupPhysicalModels : public virtual GroupId<P,MatId> {
public:
    USE_GROUP_CONSTRUCTS(GroupPhysicalModels, P);

    DEFINE_GROUP_CLONE(GroupPhysicalModels, P);

    USE_GROUP_ASSIGN(P);

    void printInfo() const;
private:
    void getDirection(PMVolumePML::Direction direction[3],
                      const UInt i) const;
    PMVolumePML::Direction getDirectionFromInt(const UInt i) const;
};

#include "GroupPhysicalModels.hpp"

typedef GroupPhysicalModels<> PMGroup;

#endif /* COMMON_PHYSICALMODEL_GROUPPHYSICALMODELS_H_ */
