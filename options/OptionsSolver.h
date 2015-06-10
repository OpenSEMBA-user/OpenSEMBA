/*
 * GlobalProblemData.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#ifndef GLOBALPROBLEMDATA_H_
#define GLOBALPROBLEMDATA_H_

#include <cmath>
#include <iostream>
#include <utility>
using namespace std;

#include "math/CartesianVector.h"

#include "Options.h"

class OptionsSolver : public Options {
public:
    enum class Solver {
        ugrfdtd, cudg3d, none
    };
    enum class CompositeModel {
        mibc, digFilt, ade, none
    };
    enum class WireModel {
        newWireModel, transitionWireModel, oldWireModel
    };
    enum class InductanceModel {
        boutayeb, ledfelt, berenger
    };
    enum class PMLBacking {
        none, mur1, mur2
    };
    enum class NoNF2FF {
        none, back, front, left, right, down, up
    };
    OptionsSolver();

    DEFINE_CLONE(OptionsSolver);

    void set(const Arguments& args);
    void setFinalTime(Real finalTime);
    void setSamplingPeriod(Real samplingPeriod);
    void setSolver(Solver solver);
    void setCFL(double cfl);
    void setCompositeModel(CompositeModel compositeModel);
    void setCompositesAttenuationFactor(double compositesAttenuationFactor);
    void setConnectEndings(bool connectEndings);
    void setGroundWires(bool groundWires);
    void setIntraWireSimplifications(bool intraWireSimplifications);
    void setIsolateGroupGroups(bool isolateGroupGroups);
    void setJoinWires(bool joinWires);
    void setMakeHoles(bool makeHoles);
    void setMTLN(bool mtln);
    void setNumberOfTimeSteps(UInt numberOfTimeSteps);
    void setPMLAlpha(const pair<double, double>& pmlAlpha);
    void setPMLCorrection(const pair<double, double>& pmlCorrection);
    void setPMLKappa(double pmlKappa);
    void setInductanceModel(InductanceModel inductanceModel);
    void setTaparrabos(bool taparrabos);
    void setWireModel(WireModel wireModel);
    void setWiresAttenuationFactor(double wiresAttenuationFactor);
    void setTimeStep(Real timeStep);

    Real getFinalTime() const;
    Real getSamplingPeriod() const;
    Solver getSolver() const;
    Real getTimeStep() const;
    double getCFL() const;
    CompositeModel getCompositeModel() const;
    double getCompositesAttenuationFactor() const;
    bool isConnectEndings() const;
    bool isGroundWires() const;
    bool isIntraWireSimplifications() const;
    bool isIsolateGroupGroups() const;
    bool isJoinWires() const;
    bool isMakeHoles() const;
    bool isMTLN() const;
    UInt getNumberOfTimeSteps() const;
    const pair<double, double>& getPmlAlpha() const;
    const pair<double, double>& getPmlCorrection() const;
    double getPmlKappa() const;
    InductanceModel getInductanceModel() const;
    bool isTaparrabos() const;
    WireModel getWireModel() const;
    double getWiresAttenuationFactor() const;

    string toArgsStr() const;
    void printInfo() const;
    void printHelp() const;

private:
    // Global
    Solver solver_;
    Real finalTime_;
    Real timeStep_;
    Real cfl_;
    Real samplingPeriod_;
    bool forceRestarting_;
    bool resumeSimulation_;
    Real flush_;

    // ugrfdtd
    CompositeModel compositeModel_;
    bool conformalSkin_;
    bool noCompoMur_;
    bool skinDepth_;
    double compositesAttenuationFactor_;

    WireModel wireModel_;
    bool MTLN_;
    Real minDistanceWires_;
    bool newDispersiveFormulation_;
    bool taparrabos_;
    bool makeHoles_;
    bool groundWires_;
    InductanceModel inductanceModel_;
    Real wiresAttenuationFactor_;

    bool useDefaultPML_;
    pair<Real,Real> pmlAlpha_;
    Real pmlKappa_;
    pair<Real,Real> pmlCorrection_;
    PMLBacking pmlBacking_;

    bool map_;
    bool MapVTK_;
    NoNF2FF noNF2FF_;
    bool nF2FFDecimation_;
    string additionalArguments_;
    //
    string toStr(const Solver solver) const;
    string toStrIfTrue(const string str, const bool param) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
