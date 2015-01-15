/*
 * OpenFOAMParameters.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef OPENFOAMPARAMETERS_H_
#define OPENFOAMPARAMETERS_H_

#include <iostream>
#include "../math/CartesianVector.h"

using namespace std;

typedef unsigned int uint;

class OpenFOAMParameters {
public:
	OpenFOAMParameters();
	OpenFOAMParameters(
	 const bool castellateMesh,
	 const bool snapMesh,
	 const bool addLayers,
	 const double edgeFeatureAngle,
	 const uint featureRefinementLevel,
	 const CVecD3& locationInMesh);
	virtual void
	 printInfo() const;
	bool
	 isCastellateMesh() const;
	bool
     isAddLayers() const;
	bool
	 isSnapMesh() const;
	double
	 getEdgeFeatureAngle() const;
	uint
	 getFeatureRefinementLevel() const;
   const CVecD3& getLocationInMesh() const;

private:
	bool castellateMesh_;
	bool snapMesh_;
	bool addLayers_;
	double edgeFeatureAngle_;
	uint featureRefinementLevel_;
	CVecD3 locationInMesh_;
	// vector<OpenFoamRefinementBox>  refinementBox_;
	// vector<OpenFoamRefinementeSphere> refinementSphere_;
};

#endif /* OPENFOAMPARAMETERS_H_ */
