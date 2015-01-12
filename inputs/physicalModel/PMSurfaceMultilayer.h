/*
 * PMMultilayer.h
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_
#define SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_

#include "PMSurface.h"

class PMSurfaceMultilayer : public PMSurface {
public:
   PMSurfaceMultilayer();
   PMSurfaceMultilayer(
         const uint id,
         const string& name,
         const vector<double>& thickness,
         const vector<double>& relPermittivity,
         const vector<double>& relPermeability,
         const vector<double>& elecCond,
         const vector<double>& magnCond);
   uint
   countLayers() const;
   string
   printLayer(const uint i) const;
private:
   vector<double>
    thickness_, relPermittivity_, relPermeability_, elecCond_, magnCond_;
};

#endif /* SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_ */
