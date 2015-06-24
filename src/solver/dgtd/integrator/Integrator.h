/*
 * SolverInfo.h
 *
 *  Created on: Feb 21, 2013
 *      Author: luis
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <vector>
#include <utility>
#include <limits>

using namespace std;

#include "../core/Comm.h"
#include "../core/Ordering.h"
#include "physicalModel/PhysicalModel.h"
#include "options/OptionsSolverDGTD.h"
#include "MeshVolume.h"
#include "DG/DG.h"

#define SOLVERINFO_ALLOW_REORDERING_IN_SOLVER

typedef pair<UInt,UInt> Interval;

class Integrator : public Ordering {
public:
    Real timeStepSize;
    Integrator();
    virtual ~Integrator();
    virtual void timeIntegrate(const Real timer) const = 0;
    void setSolver(DG* solver);
    Real getMaxDT() const;
    Real getMinDT() const;
    UInt getNTiers() const;
    UInt getNPartitions() const;
    vector<vector<ElementId>> getTiersIds() const;
    vector<vector<ElementId>> getStagesIds() const;
    vector<vector<ElementId>> getPartitionsIds() const;
    Interval getRange(const UInt tier, const UInt stage) const;
    vector<pair<ElementId,Int>> getComputationalWeights(
            const MeshVolume* msh) const;
    void partitionate(const MeshVolume* mesh, Comm* comm);
    void printInfo() const;
protected:
    DG* solver;
    bool doLTS;
    DynMatrix<UInt> timeTierList; // Id - Tier - Stage
    Real mindt;
    void init(
            const MeshVolume& mesh,
            const PMGroup& pmGroup,
            const OptionsSolverDGTD* arg);
    UInt getNumberOfCellsInTier(const UInt tier) const;
    virtual UInt getNumOfIterationsPerBigTimeStep(
            const UInt e) const = 0;
    virtual UInt getNStages() const = 0;
    virtual Real getMaxTimeStep(
            const VolR* tet,
            const PhysicalModel* mat) const;
    virtual Real getMaxTimeRatio() const = 0;
    vector<ElementId> getIdsOfTier(const UInt tier) const;
    vector<ElementId> getIdsOfStage(const UInt stage) const;
private:
    static constexpr Real cfl = 3e-9;
    static const UInt noTier = 0;
    static const UInt noStage = 0;
    static const UInt growStages = 1;
    UInt growSmallerTiers;
    UInt maxNumOfTiers;
    UInt nTiers;
    pair<UInt,UInt> **tierRange_;
    vector<vector<ElementId>> partIds_;
    void reorder(
            const vector<vector<ElementId>>& partitionsIds_,
            const UInt localOffset,
            const UInt localSize);
    void buildTierInfo(
            const MeshVolume& mesh,
            const PMGroup& pmGroup);
    virtual void
    checkMaterialStabilityForDT(
            const PhysicalModel* mat,
            const Real dt) const;
    void assignTiersBasedOnMaxTimeStep(
            const MeshVolume& mesh,
            const PMGroup& pmGroup);
    pair<UInt,UInt>** buildTierRange(
            pair<UInt,UInt> **range,
            const DynMatrix<UInt>& list);
    void growSmallestTierRegions(
            const UInt numToGrow,
            const MeshVolume& mesh);
    vector<pair<UInt, UInt> > getIdPartitionVector(
            const vector<vector<UInt> >& pId) const;
    void assignStages(const MeshVolume& mesh);
    void reorderTimeTierList(
            const vector<vector<UInt>>& partitionId);
};

#endif /* SOLVERINFO_H_ */
