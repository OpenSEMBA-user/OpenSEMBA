/*
 * OpenFOAMParameters.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "MeshingParameters.h"

MeshingParameters::MeshingParameters() {
   castellateMesh_ = false;
   snapMesh_ = false;
   addLayers_ = false;
   edgeFeatureAngle_ = 180;
   featureRefinementLevel_ = 0;
   locationInMeshSet_ = false;
}

MeshingParameters::MeshingParameters(
      const bool castellateMesh,
      const bool snapMesh,
      const bool addLayers,
      const double edgeFeatureAngle,
      const uint featureRefinementLevel,
      const bool locationInMeshSet,
      const CVecD3& locationInMesh) {
   castellateMesh_ = castellateMesh;
   snapMesh_ = snapMesh;
   addLayers_ = addLayers;
   edgeFeatureAngle_ = edgeFeatureAngle;
   featureRefinementLevel_ = featureRefinementLevel;
   locationInMeshSet_ = locationInMeshSet;
   locationInMesh_ = locationInMesh;
}

void
MeshingParameters::printInfo() const {
   cout<< " --- OpenFOAMParameters info --- " << endl
         << " Castellated Mesh:   " << castellateMesh_ << endl
         << " Snap Mesh:          " << snapMesh_ << endl
         << " Add Layers:         " << addLayers_ << endl
         << " Edge feature Angle: " << edgeFeatureAngle_ << endl
         << " Feature refinement: " << featureRefinementLevel_ << endl
         << " Location in mesh:   " << locationInMesh_.toStr() << endl
         << " --- End of OpenFOAMParameters info ---" << endl;
}

bool
MeshingParameters::isAddLayers() const {
   return addLayers_;
}

bool
MeshingParameters::isCastellateMesh() const {
   return castellateMesh_;
}

double
MeshingParameters::getEdgeFeatureAngle() const {
   return edgeFeatureAngle_;
}

uint
MeshingParameters::getFeatureRefinementLevel() const {
   return featureRefinementLevel_;
}

bool
MeshingParameters::isSnapMesh() const {
   return snapMesh_;
}

const CVecD3& MeshingParameters::getLocationInMesh() const {
   return locationInMesh_;
}

bool MeshingParameters::isLocationInMeshSet() const {
   return locationInMeshSet_;
}
