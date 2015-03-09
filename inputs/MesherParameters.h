/*
 * OpenFOAMParameters.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef OPENFOAMPARAMETERS_H_
#define OPENFOAMPARAMETERS_H_

#include <string>

#include "../math/CartesianVector.h"

using namespace std;

typedef unsigned int uint;

class MesherParameters {
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
    MesherParameters();
    MesherParameters(
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
            string swfForze,
            string confOutput);
    Mesher getMesher() const;
    void setMesher(Mesher mesher);
    virtual const CVecD3& getLocationInMesh() const;
    virtual bool isLocationInMeshSet() const;
    Mode getMode() const;
    void setMode(Mode mode);
    void setBruteForceVolumes(bool bruteForceVolumes);
    bool isStructured() const;
    bool isRelaxed() const;
    bool isSlanted() const;
    bool isBruteForceVolumes() const;
    bool isEffectiveParameter() const;
    void setEffectiveParameter(bool effectiveParameter);
    bool hasEffParams() const;
    string getEffThick() const;
    void setTh(const string& th);
    string getEffSigma() const;
    void setSigma(const string& sigma);
    string getEdgeFraction() const;
    void setEdgeFraction(const string& edgeFraction);
    bool hasScaleFactor() const;
    string getScaleFactor() const;
    void setScaleFactor(bool scaleFactor);
    void setScaleFactorValue(const string& scaleFactorValue);
    string getSWFForce() const;
    void setSwfForze(const string& swfForze);
    void setLocationInMesh(const CVecD3& locationInMesh);
    const string& getMeshOutputName() const;
    void setConfOutput(const string& confOutput);
    virtual void printInfo() const;

private:
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
