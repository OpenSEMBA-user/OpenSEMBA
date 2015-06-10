/*
 * SolverExplicit.h
 *
 *  Created on: Nov 30, 2012
 *      Author: luis
 */

#ifndef SOLVEREXPLICIT_H_

#include "DG.h"
#include "SolverMath.h"
#include "DGCurvedFace.h"

class DGExplicit : public DG {
	friend class IntegratorLSERK;
	friend class IntegratorLF2;
	friend class IntegratorLF2Full;
	friend class IntegratorVerlet;
public:
	DGExplicit(
	 const SmbData* smb,
	 Comm* comm,
	 const OptionsSolverDGTD* arg);
	virtual ~DGExplicit();
    uint
     getFieldDOFs();
	const Field<double,3>&
	 getRHSElectric() const;
	const Field<double,3>&
	 getRHSMagnetic() const;
	void
	 printInfo() const;
protected:
	void
	 computeRHS(
	  const uint e1,
	  const uint e2,
	  const double localtime,
	  const double rkdt);
	void
	 computeRHSElectric(
	  const uint e1,
	  const uint e2,
	  const double localtime,
	  const double minDT);
	void
	 computeRHSMagnetic(
	  const uint e1,
	  const uint e2,
	  const double localtime,
	  const double minDT);
	void
	 computeCurlsInRHSElectric(const uint e1, const uint e2);
	void
	 computeCurlsInRHSMagnetic(const uint e1, const uint e2);
	void
	 computeJumps(
	  const uint e1,
	  const uint e2,
	  const double localTime,
	  const double minDT);
	void
	 copyJumpsToResidueJumps(
	  const uint e1,
	  const uint e2);
	void
	 addFluxesToRHSElectric(
	  const uint e1, const uint e2);
	void
	 addFluxesToRHSMagnetic(
	  const uint e1, const uint e2);
	void
	 addFluxesToRHSElectric(
	  const uint e1, const uint e2, const bool useResForUpw);
	void
	 addFluxesToRHSMagnetic(
	  const uint e1, const uint e2, const bool useResForUpw);
	void
	 addStraightFluxesToRHSElectric(const uint e1, const uint e2,
	  const bool useResForUpw);
	void
	 addStraightFluxesToRHSMagnetic(const uint e1, const uint e2,
  	  const bool useResForUpw);
	void
	 addCurvedFluxesToRHSElectric(const uint e1, const uint e2,
	  const bool useResForUpw);
	void
	 addCurvedFluxesToRHSMagnetic(const uint e1, const uint e2,
  	  const bool useResForUpw);
	void
	 computePolarizationCurrentsRHS(
	  const uint e1, const uint e2);
	void
	 computePolarizationCurrentsRHSElectric(
	  const uint e1, const uint e2);
	void
	 computePolarizationCurrentsRHSMagnetic(
	 const uint e1, const uint e2);
	void
	 addRHSToFieldsElectric(
	  const uint e1,
	  const uint e2,
	  const double rkdt);
	void
	 addRHSToFieldsMagnetic(
	  const uint e1,
	  const uint e2,
	  const double rkdt);
	uint
	 getIndexOfElement(const uint e) const {
		return (e * np);
	 }
	void
	 addRHSToResidueElectric(const uint e1, const uint e2,
	  const double rkdt);
	void
	 addRHSToResidueMagnetic(const uint e1, const uint e2,
	  const double rkdt);
	void
	 addRHSToRes(
	  const uint e1,
	  const uint e2,
 	  const double rka,
 	  const double dt);
	void
	 updateFieldsWithRes(
	  const uint e1,
	  const uint e2,
	  const double rkb);
	void
	 LTSSaveFieldsAndResidues(
	  const uint fKSave,
	  const uint lKSave);
	void
	 LTSLoadFieldsAndResidues(
	  const uint fKSave,
	  const uint lKSave);
private:
	// - Maps.
	int vmapM[faces][nfp];
	int vmapP[16][nfp];
	int ***map;
	// Pointers to neighbour fields. dim = (nK, 4).
	double ***ExP, ***EyP, ***EzP, ***HxP, ***HyP, ***HzP;
	// Curved faces stuff ---------------------------------------------
	uint nCurvedFaces;
	DGCurvedFace *curveFace;
	const double **Cx, **Cy, **Cz; // Pointers to C. dim = (nK)
	// Fields and residuals: dim = (np,nK)
	Field<double,3> rhsE, rhsH;
	Field<double,3> savedResE, savedResH;
	Field<double,3> savedE, savedH;
	Field<double,3> nE, nH;
	// Jumps and fluxes: dim = (4*nfp, nK)
	Field<double,3> dE, dH;
	Field<double,3> dresE, dresH;
	// BC lists. nSMA, nPEC and nPMC are the number of BC of each kind.
	// BC are stored as pointers to memory positions in the jumps.
	// dim = (nK)
	uint nSMA, nPEC, nPMC;
	uint *SMAe, *SMAf, *PECe, *PECf, *PMCe, *PMCf;
	vector<DGSource*> source;
	vector<DGDispersive*> dispersive;
	void
	 buildMaterials(
	  const CellGroup& cells,
	  const OptionsSolverDGTD* arg);
	void
	 deduplicateVMaps(const CellGroup& cells);
	void
	 allocateRHSAndJumps(const OptionsSolverDGTD* args);
	void
	 allocateMaps();
	void
	 assignPointersToNeighbours(
	  const CellGroup& cells,
	  const MeshVolume& mesh);
	void
	 buildScalingFactors(
	  const CellGroup& cells,
	  const MapGroup& map);
	void
	 buildEMSources(
	  const EMSourceGroup& emSource,
	  const BCGroup&,
	  const MapGroup& maps,
	  const CellGroup& cells);
	void
	 BCGroupsToLocalArray(
	  const BCGroup& bc,
	  const CellGroup& cells,
	  const MapGroup& mesh);
	vector<const BoundaryCondition*>
	 removeNonLocalBCs(
	  const CellGroup* cells,
	  const vector<const BoundaryCondition*>& bc) const;
	bool
	 checkPtrsToNeigh() const;
	void
	 assignMatrices(const CellGroup& cells);
	void
	 allocateFieldsForLTS();
	void
	 buildCurvedFluxScalingFactors(
	  const CellGroup& cells,
	  const MapGroup& map);
};

#endif /* SOLVEREXPLICIT_H_ */
