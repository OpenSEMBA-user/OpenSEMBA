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

class MesherOptions : public Options {
public:
    typedef enum {
        openfoam,
        ugrMesher,
        none
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
        symmetricETan,
        symmetricHTan,
        undefined
    } BoundType;
    MesherOptions();
    MesherOptions(
            Mesher mesher,
            bool locationInMeshSet,
            CVecR3 locationInMesh,
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

    DEFINE_CLONE(MesherOptions);

    Mesher getMesher() const;

    void setMesher(Mesher mesher);
    virtual const CVecR3& getLocationInMesh() const;
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
    void setLocationInMesh(const CVecR3& locationInMesh);
    const string& getMeshOutputName() const;
    void setConfOutput(const string& confOutput);

    const pair<CVecR3, CVecR3>& getBoundaryMeshSize() const;
    void setBoundaryMeshSize(const pair<CVecR3, CVecR3>& boundaryMeshSize);
    const pair<CVecR3, CVecR3>& getBoundaryPadding() const;
    void setBoundaryPadding(const pair<CVecR3, CVecR3>& boundaryPadding);
    Real getScalingFactor() const;
    void setScalingFactor(Real scalingFactor);
    BoundType getBoundTermination(const UInt i, const UInt p) const;
    void setBoundTermination(const UInt i, UInt j, BoundType bound);

    void applyGeometricScalingFactor(const Real& factor);
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
    CVecR3 locationInMesh_;
    string confOutput_;

    Real scalingFactor_;
    pair<BoundType,BoundType> boundTermination_[3];
    pair<CVecR3,CVecR3> boundaryPadding_, boundaryMeshSize_;
    string toStr(const BoundType) const;
};

#endif /* OPENFOAMPARAMETERS_H_ */
