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
         const MatId id,
         const string& name,
         const vector<Real>& thickness,
         const vector<Real>& relPermittivity,
         const vector<Real>& relPermeability,
         const vector<Real>& elecCond,
         const vector<Real>& magnCond);

   DEFINE_CLONE(PMSurfaceMultilayer);

   UInt getNumberOfLayers() const;
   string printLayer(const UInt i) const;
   Real getThickness(const UInt i) const;
   Real getPermittivity(const UInt i) const;
   Real getPermeability(const UInt i) const;
   Real getElecCond(const UInt i) const;
   Real getMagnCond(const UInt i) const;

   void printInfo() const;
private:
   vector<Real>
    thickness_, relPermittivity_, relPermeability_, elecCond_, magnCond_;
};

#endif /* SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_ */
