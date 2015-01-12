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

uint PMSurfaceMultilayer::countLayers() const {
   return thickness_.size();
}

string
PMSurfaceMultilayer::printLayer(const uint i) const {
   assert(i < countLayers());
   stringstream ss;
   ss << elecCond_[i] << " " << relPermittivity_[i]*eps0 << " "
      << relPermeability_[i]*mu0 << " " << magnCond_[i] << " "
      << thickness_[i];
   return string(ss.str());
}

void
PMSurfaceMultilayer::printInfo() const {
   cout << " --- PMSurfaceMultilayer info ---" << endl;
   PMSurface::printInfo();
   cout << "Number of layers: " << countLayers() << endl;
   cout <<
    "#, Thickness, Permittivity, Permeability, ElecCond, MagnCond" << endl;
   for (uint i = 0; i < countLayers(); i++) {
      cout<< i << ": "
          << thickness_[i] << " "
          << relPermittivity_[i] << " "
          << relPermeability_[i] << " "
          << elecCond_[i] << " "
          << magnCond_[i] << endl;
   }
   cout << " --- End of PMSurfaceMultilayer info ---" << endl;
}
