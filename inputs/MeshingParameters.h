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

class MeshingParameters {
public:
    typedef enum {
        none,
        openfoam,
        ugrMesher
    } Mesher;
    typedef enum {
        structured,
        relaxed,
        slanted
    } Mode;
    MeshingParameters();
    MeshingParameters(
            Mesher mesher,
            bool locationInMeshSet,
            CVecD3 locationInMesh,
            bool bruteForceVolumes,
            Mode mode,
            bool effectiveParameter,
            string th,
            string sigma,
            string edgeFraction,
            bool scaleFactor,
            string scaleFactorValue,
            string swfForze);
    virtual const CVecD3& getLocationInMesh() const;
    virtual bool isLocationInMeshSet() const;
    bool useOpenFoam() const;
    bool isBruteForceVolumes() const;
    bool isStructured() const;
    bool hasEffParams() const;
    string getEffThick() const;
    string getEffSigma() const;
    bool isRelaxed() const;
    bool isSlanted() const;
    string getEdgeFraction() const;
    bool hasScaleFactor() const;
    string getScaleFactor() const;
    string getSWFForce() const;
    virtual void
    printInfo() const;
protected:
    Mesher mesher_;
    Mode mode_;
    bool bruteForceVolumes_;
    bool effectiveParameter_;
    string th_;
    string sigma_;
    string edgeFraction_;
    bool scaleFactor_;
    string scaleFactorValue_;
    string swfForze_;
    bool locationInMeshSet_;
    CVecD3 locationInMesh_;
    string confOutput_;
};

#endif /* OPENFOAMPARAMETERS_H_ */
