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

class Integrator : public Ordering {
public:
	Real timeStepSize;
	Integrator();
	virtual ~Integrator();
	virtual void
	 timeIntegrate(const Real timer) const = 0;
	void setSolver(DG* solver);
	Real getMaxDT() const;
	Real getMinDT() const;
	UInt getNTiers() const;
	UInt getNPartitions() const;
	vector<vector<UInt> > getTiersIds() const;
	vector<vector<UInt> > getStagesIds() const;
	vector<vector<UInt> > getPartitionsIds() const;
	pair<UInt,UInt> getRange(const UInt tier, const UInt stage) const;
	vector<pair<UInt,Int> > getComputationalWeights(
	        const MeshVolume* msh) const;
	void partitionate(const MeshVolume* mesh, Comm* comm);
	void printInfo() const;
protected:
	DG* solver;
	bool doLTS;
	DynMatrix<UInt> timeTierList; // Id - Tier - Stage
	Real mindt;
	void
	 init(
	  const MeshVolume& mesh,
	  const PMGroup& pmGroup,
	  const OptionsSolverDGTD* arg);
	UInt
	 getNumberOfCellsInTier(const UInt tier) const;
	virtual UInt
 	 getNumOfIterationsPerBigTimeStep(
      const UInt e) const = 0;
	virtual UInt
	 getNStages() const = 0;
	virtual Real
	 getMaxTimeStep(
	  const Tet* tet,
	  const PhysicalModel* mat) const;
	virtual Real
	 getMaxTimeRatio() const = 0;
	vector<UInt>
	 getIdsOfTier(const UInt tier) const;
	vector<UInt>
	 getIdsOfStage(const UInt stage) const;
private:
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	static constexpr Real cfl = 3e-9;
#else
	static const Real cfl = 3e-9;
#endif
	static const UInt noTier = 0;
	static const UInt noStage = 0;
	UInt growSmallerTiers;
	static const UInt growStages = 1;
	UInt maxNumOfTiers;
	UInt nTiers;
	pair<UInt,UInt> **tierRange;
	vector<vector<UInt> > partIds;
	void
	 reorder(
      const vector<vector<UInt> >& partitionsIds_,
	  const UInt localOffset,
	  const UInt localSize);
	void
	 buildTierInfo(
	  const MeshVolume& mesh,
	  const PMGroup& pmGroup);
	virtual void
	 checkMaterialStabilityForDT(
	  const PhysicalModel* mat,
	  const Real dt) const;
	void
	 assignTiersBasedOnMaxTimeStep(
	  const MeshVolume& mesh,
	  const PMGroup& pmGroup);
	pair<UInt,UInt>**
	 buildTierRange(
 	  pair<UInt,UInt> **range,
	  const DynMatrix<UInt>& list);
	void
	 growSmallestTierRegions(
	  const UInt numToGrow,
	  const MeshVolume& mesh);
	vector<pair<UInt, UInt> >
	getIdPartitionVector(
	 const vector<vector<UInt> >& pId) const;
	void
	 assignStages(const MeshVolume& mesh);
//	void
//	 processStopRequest();
//	Int
//	 kbhit();
//	void
// 	 nonblock(Int state);
	void
	 reorderTimeTierList(
	  const vector<vector<UInt> >& partitionId);
};

#endif /* SOLVERINFO_H_ */
