/*
 * OpenFOAMParameters.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef OPENFOAMPARAMETERS_H_
#define OPENFOAMPARAMETERS_H_

#include <cstdlib>
#include <string>

using namespace std;

#include "math/CartesianVector.h"
#include "Options.h"

class OptionsMesher : public Options {
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
    OptionsMesher();
    OptionsMesher(
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

    DEFINE_CLONE(OptionsMesher);

    virtual void set(const Arguments& args);
    void setBoundaryMeshSize(const pair<CVecR3, CVecR3>& boundaryMeshSize);
    void setBoundaryPadding(const pair<CVecR3, CVecR3>& boundaryPadding);
    void setScalingFactor(Real scalingFactor);
    void setBoundTermination(const UInt i, UInt j, BoundType bound);
    void setBruteForceVolumes(bool bruteForceVolumes);
    void setConfOutput(const string& confOutput);
    void setEdgeFraction(const string& edgeFraction);
    void setLocationInMesh(const CVecR3& locationInMesh);
    void setLocationInMeshSet(bool locationInMeshSet);
    void setMesher(Mesher mesher);
    void setMode(Mode mode);
    void setScaleFactor(bool scaleFactor);
    void setScaleFactorValue(const string& scaleFactorValue);
    void setSigma(const string& sigma);
    void setSwfForze(const string& swfForze);
    void setEffectiveParameter(bool effectiveParameter);
    void applyGeometricScalingFactor(const Real& factor);

    Mesher getMesher() const;
    virtual const CVecR3& getLocationInMesh() const;
    virtual bool isLocationInMeshSet() const;
    Mode getMode() const;
    bool isStructured() const;
    bool isRelaxed() const;
    bool isSlanted() const;
    bool isBruteForceVolumes() const;
    bool isEffectiveParameter() const;
    string getEdgeFraction() const;
    bool hasScaleFactor() const;
    string getScaleFactor() const;
    string getSWFForce() const;
    const string& getMeshOutputName() const;

    const pair<CVecR3, CVecR3>& getBoundaryMeshSize() const;
    const pair<CVecR3, CVecR3>& getBoundaryPadding() const;
    Real getScalingFactor() const;
    vector<pair<BoundType,BoundType>> getBoundTerminations() const;
    BoundType getBoundTermination(const UInt i, const UInt p) const;

    void printHelp() const;
    void printInfo() const;
    static string toStr(const BoundType);

private:
    Mesher mesher_;
    Mode mode_;
    bool bruteForceVolumes_;
    string edgeFraction_;
    bool scaleFactor_;
    string scaleFactorValue_;
    string swfForze_;
    bool locationInMeshSet_;
    CVecR3 locationInMesh_;
    string confOutput_;

    Real scalingFactor_;
    vector<pair<BoundType,BoundType>> boundTermination_;
    pair<CVecR3,CVecR3> boundaryPadding_, boundaryMeshSize_;
};

typedef vector<pair<OptionsMesher::BoundType,OptionsMesher::BoundType>>
        BoundTerminations;

#endif /* OPENFOAMPARAMETERS_H_ */
