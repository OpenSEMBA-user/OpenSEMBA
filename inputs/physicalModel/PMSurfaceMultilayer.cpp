/*
 * PMMultilayer.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#include "PMSurfaceMultilayer.h"

PMSurfaceMultilayer::PMSurfaceMultilayer(
      const MatId id,
      const string& name,
      const vector<Real>& thickness,
      const vector<Real>& relPermittivity,
      const vector<Real>& relPermeability,
      const vector<Real>& elecCond,
      const vector<Real>& magnCond) : PMSurface(id, name) {
   thickness_ = thickness;
   relPermittivity_ = relPermittivity;
   relPermeability_ = relPermeability;
   elecCond_ = elecCond;
   magnCond_ = magnCond;
   const UInt nLayers = thickness_.size();
   if (relPermittivity_.size() != nLayers
         || relPermeability_.size() != nLayers
         || elecCond_.size() != nLayers
         || magnCond_.size() != nLayers) {
      cerr << "ERROR @ PMSurfaceMultilayer: "
           << "Incompatible sizes of layers parameters. "
           << "In multilayer: " << endl;
      printInfo();
      assert(false);
   }
}

ClassBase* PMSurfaceMultilayer::clone() const {
   return new PMSurfaceMultilayer(*this);
}

UInt PMSurfaceMultilayer::getNumberOfLayers() const {
   return thickness_.size();
}

string
PMSurfaceMultilayer::printLayer(const UInt i) const {
   assert(i < getNumberOfLayers());
   stringstream ss;
   ss << elecCond_[i] << " " << relPermittivity_[i]*Constants::eps0 << " "
      << relPermeability_[i] * Constants::mu0 << " " << magnCond_[i] << " "
      << thickness_[i];
   return string(ss.str());
}

Real
PMSurfaceMultilayer::getThickness(const UInt i) const {
   return thickness_[i];
}

Real PMSurfaceMultilayer::getPermittivity(const UInt i) const {
   return relPermittivity_[i] * Constants::eps0;
}

Real PMSurfaceMultilayer::getPermeability(const UInt i) const {
   return relPermeability_[i] * Constants::mu0;
}

Real PMSurfaceMultilayer::getElecCond(const UInt i) const {
   return elecCond_[i];
}

Real PMSurfaceMultilayer::getMagnCond(const UInt i) const {
   return magnCond_[i];
}

void
PMSurfaceMultilayer::printInfo() const {
   cout << " --- PMSurfaceMultilayer info ---" << endl;
   PMSurface::printInfo();
   cout << "Number of layers: " << getNumberOfLayers() << endl;
   cout <<
    "#, Thickness, Permittivity, Permeability, ElecCond, MagnCond" << endl;
   for (UInt i = 0; i < getNumberOfLayers(); i++) {
      cout<< i << ": "
          << thickness_[i] << " "
          << relPermittivity_[i] << " "
          << relPermeability_[i] << " "
          << elecCond_[i] << " "
          << magnCond_[i] << endl;
   }
   cout << " --- End of PMSurfaceMultilayer info ---" << endl;
}
