/*
 * GlobalProblemData.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#ifndef GLOBALPROBLEMDATA_H_
#define GLOBALPROBLEMDATA_H_

#include <iostream>
#include <utility>
#include "../math/CartesianVector.h"
#include "Options.h"

using namespace std;

typedef unsigned int uint;

class SolverOptions : public Options {
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
    SolverOptions();

    double getFinalTime() const;
    void setFinalTime(double finalTime);
    double getSamplingPeriod() const;
    void setSamplingPeriod(double samplingPeriod);
    Solver getSolver() const;
    void setSolver(Solver solver);
    double getTimeStep() const;
    void setTimeStep(double timeStep);

    void printInfo() const;
    double getCfl() const;
    void setCFL(double cfl);
    CompositeModel getCompositeModel() const;
    void setCompositeModel(CompositeModel compositeModel);
    double getCompositesAttenuationFactor() const;
    void setCompositesAttenuationFactor(double compositesAttenuationFactor);
    bool isConnectEndings() const;
    void setConnectEndings(bool connectEndings);
    bool isGroundWires() const;
    void setGroundWires(bool groundWires);
    bool isIntraWireSimplifications() const;
    void setIntraWireSimplifications(bool intraWireSimplifications);
    bool isIsolateGroupGroups() const;
    void setIsolateGroupGroups(bool isolateGroupGroups);
    bool isJoinWires() const;
    void setJoinWires(bool joinWires);
    bool isMakeHoles() const;
    void setMakeHoles(bool makeHoles);
    MetalModel getMetalModel() const;
    void setMetalModel(MetalModel metalModel);
    bool isMTLN() const;
    void setMTLN(bool mtln);
    uint getNumberOfTimeSteps() const;
    void setNumberOfTimeSteps(uint numberOfTimeSteps);
    const pair<double, double>& getPmlAlpha() const;
    void setPMLAlpha(const pair<double, double>& pmlAlpha);
    const pair<double, double>& getPmlCorrection() const;
    void setPMLCorrection(const pair<double, double>& pmlCorrection);
    double getPmlKappa() const;
    void setPMLKappa(double pmlKappa);
    SelfInductanceModel getSelfInductanceModel() const;
    void setSelfInductanceModel(SelfInductanceModel selfInductanceModel);
    bool isTaparrabos() const;
    void setTaparrabos(bool taparrabos);
    WireModel getWireModel() const;
    void setWireModel(WireModel wireModel);
    double getWiresAttenuationFactor() const;
    void setWiresAttenuationFactor(double wiresAttenuationFactor);

private:
    // Global
    Solver solver_;
    double finalTime_;
    uint numberOfTimeSteps_;
    double samplingPeriod_;
    double timeStep_;
    double cfl_;
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
