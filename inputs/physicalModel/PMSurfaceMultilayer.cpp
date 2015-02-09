/*
 * PMMultilayer.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#include "PMSurfaceMultilayer.h"

PMSurfaceMultilayer::PMSurfaceMultilayer() {
}

PMSurfaceMultilayer::PMSurfaceMultilayer(
      const uint id,
      const string& name,
      const vector<double>& thickness,
      const vector<double>& relPermittivity,
      const vector<double>& relPermeability,
      const vector<double>& elecCond,
      const vector<double>& magnCond) : PMSurface(id, name) {
   thickness_ = thickness;
   relPermittivity_ = relPermittivity;
   relPermeability_ = relPermeability;
   elecCond_ = elecCond;
   magnCond_ = magnCond;
   const uint nLayers = thickness_.size();
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

uint PMSurfaceMultilayer::getNumberOfLayers() const {
   return thickness_.size();
}

string
PMSurfaceMultilayer::printLayer(const uint i) const {
   assert(i < getNumberOfLayers());
   stringstream ss;
   ss << elecCond_[i] << " " << relPermittivity_[i]*eps0 << " "
      << relPermeability_[i]*mu0 << " " << magnCond_[i] << " "
      << thickness_[i];
   return string(ss.str());
}

double
PMSurfaceMultilayer::getThickness(const uint i) const {
   return thickness_[i];
}

double PMSurfaceMultilayer::getPermittivity(const uint i) const {
   return relPermittivity_[i] * eps0;
}

double PMSurfaceMultilayer::getPermeability(const uint i) const {
   return relPermeability_[i] * mu0;
}

double PMSurfaceMultilayer::getElecCond(const uint i) const {
   return elecCond_[i];
}

double PMSurfaceMultilayer::getMagnCond(const uint i) const {
   return magnCond_[i];
}

void
PMSurfaceMultilayer::printInfo() const {
   cout << " --- PMSurfaceMultilayer info ---" << endl;
   PMSurface::printInfo();
   cout << "Number of layers: " << getNumberOfLayers() << endl;
   cout <<
    "#, Thickness, Permittivity, Permeability, ElecCond, MagnCond" << endl;
   for (uint i = 0; i < getNumberOfLayers(); i++) {
      cout<< i << ": "
          << thickness_[i] << " "
          << relPermittivity_[i] << " "
          << relPermeability_[i] << " "
          << elecCond_[i] << " "
          << magnCond_[i] << endl;
   }
   cout << " --- End of PMSurfaceMultilayer info ---" << endl;
}
