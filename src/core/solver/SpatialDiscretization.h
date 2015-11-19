/*
 * SpatialDiscretization.h
 *
 *  Created on: Nov 19, 2015
 *      Author: luis
 */

#ifndef SRC_CORE_SOLVER_SPATIALDISCRETIZATION_H_
#define SRC_CORE_SOLVER_SPATIALDISCRETIZATION_H_

#include <map>
using namespace std;

#include "base/group/Group.h"
#include "exporter/Output.h"

class SpatialDiscretization {
public:
    SpatialDiscretization();
    virtual ~SpatialDiscretization();

    void setOutputs(const GroupOutRqs& outRqs);

    void updateOutputs(Group<OutputBase>& outputs) const;

protected:
    map<ElementId,vector<const Cell*>> outputIdsToCells_;
    //!< Index of Output requests ElementIds to solver cells.
};

#endif /* SRC_CORE_SOLVER_SPATIALDISCRETIZATION_H_ */
