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
class PhysicalModelGroup : public GroupId<P,uint> {
public:
    typedef enum {
        vacuum = 1,
        PEC = 2,
        PMC = 3,
        SMA = 4,
        classic = 5,
        elecDispersive = 6,
        isotropicsibc = 7,
        PML = 8,
        wire = 9,
        multiport = 10,
        undefined = 0
    } Type;

    PhysicalModelGroup();
    PhysicalModelGroup(const vector<P*>&);
    PhysicalModelGroup(const Group<P>& rhs);
    virtual ~PhysicalModelGroup();

    PhysicalModelGroup<P>& operator=(const Group<P>& rhs);

    vector<uint> getIds(
            const Condition::Type type = Condition::undefined) const;
    void printInfo() const;
private:
    void getDirection(
            PMVolumePML::Direction direction[3],
            const uint i) const;
    PMVolumePML::Direction getDirectionFromInt(const uint i) const;
};

#include "PhysicalModelGroup.hpp"

#endif /* PHYSICALMODELGROUP_H_ */
