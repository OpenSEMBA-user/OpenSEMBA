/*
 * SmbData.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#include "SmbData.h"

SmbData::SmbData() {
   layers = NULL;
   mesh = NULL;
   solverOptions = NULL;
   pMGroup = NULL;
   emSources = NULL;
   outputRequests = NULL;
   mesherOptions = NULL;
}

SmbData::~SmbData() {
}

SmbData&
SmbData::operator=(const SmbData& rhs) {
   if (this == &rhs) {
      return *this;
   }
   layers = new LayerGroup<>(*rhs.layers);
   mesh = new Mesh(*rhs.mesh);
   solverOptions = new SolverOptions(*rhs.solverOptions);
   pMGroup = new PhysicalModelGroup<>(*rhs.pMGroup);
   emSources = new EMSourceGroup<>(*rhs.emSources);
   outputRequests = new OutRqGroup<>(*rhs.outputRequests);
   mesherOptions = new MesherOptions(*rhs.mesherOptions);
   return *this;
}

void
SmbData::printInfo() const {
   cout << " --- SEMBA data --- " << endl;
   if (layers != NULL) {
      layers->printInfo();
   } else {
      cout << "No info about layers." << endl;
   }
   if (mesh != NULL) {
      mesh->printInfo();
   } else {
      cout << "No info about mesh." << endl;
   }
   if (solverOptions != NULL) {
      solverOptions->printInfo();
   } else {
      cout << "No info about global data." << endl;
   }
   if (pMGroup != NULL) {
      pMGroup->printInfo();
   } else {
      cout << "No info about physical models." << endl;
   }
   if (emSources != NULL) {
      emSources->printInfo();
   } else {
      cout << "No info about electromagnetic sources." << endl;
   }
   if (outputRequests != NULL) {
      outputRequests->printInfo();
   } else {
      cout << "No info about output requests." << endl;
   }
   if (mesherOptions != NULL) {
      mesherOptions->printInfo();
   } else {
      cout << "No info about mesher options." << endl;
   }
}

void
SmbData::applyGeometricScalingFactor() {
   mesh->applyGeometricScalingFactor(mesherOptions->getScalingFactor());
   mesherOptions->applyGeometricScalingFactor(mesherOptions->getScalingFactor());
   mesherOptions->setScalingFactor((Real) 1.0);
}
