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
#include "../math/CartesianVector.h"
#include "Options.h"

using namespace std;

class OptionsSolver : public Options {
public:
    enum class Solver {
        ugrfdtd, cudg3d, none
    };
    enum class CompositeModel {
        Default, digFilt, MIBC, ADEMIBC, URMMMT
    };
    enum class MetalModel {
        Default, maloney, maloneySkinDepth, conformalSkinDepth
    };
    enum class WireModel {
        Default, transition, New
    };
    enum class SelfInductanceModel {
        boutayeb, ledfelt, berenger
    };
    OptionsSolver();

    DEFINE_CLONE(OptionsSolver);

    void set(Arguments& args);
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
    void setMetalModel(MetalModel metalModel);
    void setMTLN(bool mtln);
    void setNumberOfTimeSteps(UInt numberOfTimeSteps);
    void setPMLAlpha(const pair<double, double>& pmlAlpha);
    void setPMLCorrection(const pair<double, double>& pmlCorrection);
    void setPMLKappa(double pmlKappa);
    void setSelfInductanceModel(SelfInductanceModel selfInductanceModel);
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
    MetalModel getMetalModel() const;
    bool isMTLN() const;
    UInt getNumberOfTimeSteps() const;
    const pair<double, double>& getPmlAlpha() const;
    const pair<double, double>& getPmlCorrection() const;
    double getPmlKappa() const;
    SelfInductanceModel getSelfInductanceModel() const;
    bool isTaparrabos() const;
    WireModel getWireModel() const;
    double getWiresAttenuationFactor() const;

    void printInfo() const;
    void printHelp() const;

private:
    // Global
    Solver solver_;
    Real finalTime_;
    Real timeStep_;
    Real cfl_;
    Real samplingPeriod_;
    // ugrfdtd
    CompositeModel compositeModel_;
    double compositesAttenuationFactor_;
    MetalModel metalModel_;
    pair<double,double> pmlAlpha_;
    double pmlKappa_;
    pair<double,double> pmlCorrection_;
    WireModel wireModel_;
    SelfInductanceModel selfInductanceModel_;
    bool taparrabos_;
    bool intraWireSimplifications_;
    bool MTLN_;
    bool joinWires_;
    bool groundWires_;
    bool connectEndings_;
    bool isolateGroupGroups_;
    bool makeHoles_;
    double wiresAttenuationFactor_;
    //
    string toStr(const Solver solver) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
