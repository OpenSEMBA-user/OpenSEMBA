/*
 * PMSurface.h
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACE_H_
#define SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACE_H_

#include "PhysicalModel.h"

class PMSurface: public PhysicalModel {
public:
   PMSurface(const uint id, const string& name);
   virtual bool
    isSurface() const {return true;}
};

#endif /* SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACE_H_ */
