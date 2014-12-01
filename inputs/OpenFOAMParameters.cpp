/*
 * OpenFOAMParameters.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "OpenFOAMParameters.h"

OpenFOAMParameters::OpenFOAMParameters() {
	castellateMesh_ = false;
	snapMesh_ = false;
	addLayers_ = false;
	edgeFeatureAngle_ = 180;
	featureRefinementLevel_ = 0;
}

OpenFOAMParameters::OpenFOAMParameters(
 const bool castellateMesh,
 const bool snapMesh,
 const bool addLayers,
 const double edgeFeatureAngle,
 const uint featureRefinementLevel) {
	castellateMesh_ = castellateMesh;
	snapMesh_ = snapMesh;
	addLayers_ = addLayers;
	edgeFeatureAngle_ = edgeFeatureAngle;
	featureRefinementLevel_ = featureRefinementLevel;
}

OpenFOAMParameters::~OpenFOAMParameters() {
	// TODO Auto-generated destructor stub
}

void
OpenFOAMParameters::printInfo() const {
	cout<< " --- OpenFOAMParameters info --- " << endl
	    << " Castellated Mesh:   " << castellateMesh_ << endl
	    << " Snap Mesh:          " << snapMesh_ << endl
	    << " Add Layers:         " << addLayers_ << endl
	    << " Edge feature Angle: " << edgeFeatureAngle_ << endl
	    << " Feature refinement: " << featureRefinementLevel_ << endl
	    << " --- End of OpenFOAMParameters info ---" << endl;
}

bool
OpenFOAMParameters::isAddLayers() const {
	return addLayers_;
}

bool
OpenFOAMParameters::isCastellateMesh() const {
	return castellateMesh_;
}

double
OpenFOAMParameters::getEdgeFeatureAngle() const {
	return edgeFeatureAngle_;
}

uint
OpenFOAMParameters::getFeatureRefinementLevel() const {
	return featureRefinementLevel_;
}

bool
OpenFOAMParameters::isSnapMesh() const {
	return snapMesh_;
}
