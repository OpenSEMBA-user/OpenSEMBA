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
#include "Options.h"

using namespace std;

typedef unsigned int uint;

class MesherOptions : public Options {
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
    typedef enum {
        pec,
        pmc,
        pml,
        mur1,
        mur2,
        periodic,
        undefined
    } BoundType;
    MesherOptions();
    MesherOptions(
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

    const pair<CVecD3, CVecD3>& getBoundaryMeshSize() const;
    void setBoundaryMeshSize(const pair<CVecD3, CVecD3>& boundaryMeshSize);
    const pair<CVecD3, CVecD3>& getBoundaryPadding() const;
    void setBoundaryPadding(const pair<CVecD3, CVecD3>& boundaryPadding);
    double getScalingFactor() const;
    void setScalingFactor(double scalingFactor);
    pair<BoundType,BoundType> getBoundTermination(const uint i) const;
    void setBoundTermination(const uint i, uint j, BoundType bound);

    void applyGeometricScalingFactor(const double& factor);
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

    double scalingFactor_;
    pair<BoundType,BoundType> boundTermination_[3];
    pair<CVecD3,CVecD3> boundaryPadding_, boundaryMeshSize_;
    string toStr(const BoundType) const;
};

#endif /* OPENFOAMPARAMETERS_H_ */
