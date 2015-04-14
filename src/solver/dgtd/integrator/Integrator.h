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
#include "../core/ArgumentsCudg3d.h"
#include "../core/Comm.h"
#include "../core/Ordering.h"
#include "../../../common/inputs/physicalModel/PhysicalModel.h"
#include "../../../common/geometry/MeshVolume.h"
#include "DG/DG.h"

#define SOLVERINFO_ALLOW_REORDERING_IN_SOLVER

#ifndef SOLVERINFO_ERROR
	#define SOLVERINFO_ERROR 111444
#endif

using namespace std;

class Integrator : public Ordering {
public:
	double timeStepSize;
	Integrator();
	virtual ~Integrator();
	virtual void
	 timeIntegrate(const double timer) const = 0;
	void
	 setSolver(DG* solver);
	double
	 getMaxDT() const;
	double
	 getMinDT() const;
	uint
	 getNTiers() const;
	uint
	 getNPartitions() const;
	vector<vector<uint> >
	 getTiersIds() const;
	vector<vector<uint> >
	 getStagesIds() const;
	vector<vector<uint> >
	 getPartitionsIds() const;
	pair<uint,uint>
	 getRange(const uint tier, const uint stage) const;
	vector<pair<uint,int> >
	 getComputationalWeights(const MeshVolume* msh) const;
	void
	 partitionate(
	  const MeshVolume* mesh,
	  Comm* comm);
	void
	 printInfo() const;
protected:
	DG* solver;
	bool doLTS;
	DynMatrix<uint> timeTierList; // Id - Tier - Stage
	double mindt;
	void
	 init(
	  const MeshVolume& mesh,
	  const PhysicalModelGroup& pmGroup,
	  const ArgumentsDGTD* arg);
	uint
	 getNumberOfCellsInTier(const uint tier) const;
	virtual uint
 	 getNumOfIterationsPerBigTimeStep(
      const uint e) const = 0;
	virtual uint
	 getNStages() const = 0;
	virtual double
	 getMaxTimeStep(
	  const Tet* tet,
	  const PhysicalModel* mat) const;
	virtual double
	 getMaxTimeRatio() const = 0;
	vector<uint>
	 getIdsOfTier(const uint tier) const;
	vector<uint>
	 getIdsOfStage(const uint stage) const;
private:
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	static constexpr double cfl = 3e-9;
#else
	static const double cfl = 3e-9;
#endif
	static const uint noTier = 0;
	static const uint noStage = 0;
	uint growSmallerTiers;
	static const uint growStages = 1;
	uint maxNumOfTiers;
	uint nTiers;
	pair<uint,uint> **tierRange;
	vector<vector<uint> > partIds;
	void
	 reorder(
      const vector<vector<uint> >& partitionsIds_,
	  const uint localOffset,
	  const uint localSize);
	void
	 buildTierInfo(
	  const MeshVolume& mesh,
	  const PhysicalModelGroup& pmGroup);
	virtual void
	 checkMaterialStabilityForDT(
	  const PhysicalModel* mat,
	  const double dt) const;
	void
	 assignTiersBasedOnMaxTimeStep(
	  const MeshVolume& mesh,
	  const PhysicalModelGroup& pmGroup);
	pair<uint,uint>**
	 buildTierRange(
 	  pair<uint,uint> **range,
	  const DynMatrix<uint>& list);
	void
	 growSmallestTierRegions(
	  const uint numToGrow,
	  const MeshVolume& mesh);
	vector<pair<uint, uint> >
	getIdPartitionVector(
	 const vector<vector<uint> >& pId) const;
	void
	 assignStages(const MeshVolume& mesh);
//	void
//	 processStopRequest();
//	int
//	 kbhit();
//	void
// 	 nonblock(int state);
	void
	 reorderTimeTierList(
	  const vector<vector<uint> >& partitionId);
};

#endif /* SOLVERINFO_H_ */
