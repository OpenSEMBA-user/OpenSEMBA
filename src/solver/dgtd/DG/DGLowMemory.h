/*
 * SolverLM.h
 *
 *  Created on: Sep 3, 2014
 *      Author: luis
 */

#ifndef DGLOWMEMORY_H_
#define DGLOWMEMORY_H_

#include "DG.h"

class DGLowMemory: public DG {
public:
	DGLowMemory(
	 const SmbData* smb,
	 const CellGroup* cells,
	 const BCGroup* bc,
	 Comm* comm,
	 const ArgumentsDGTD* arg);
	virtual ~DGLowMemory();
	void
	 printInfo() const;
protected:
	// Computes residue
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
	 addFluxesToRHSElectric(
	  const uint e1, const uint e2);
	void
	 addFluxesToRHSMagnetic(
	  const uint e1, const uint e2);
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
	 addRHSToRes(
	  const uint e1,
	  const uint e2,
	  const double rka,
	  const double dt);
	void
	 addRHSToResidueElectric(
	  const uint e1, const uint e2, const double rkdt);
	void
	 addRHSToResidueMagnetic(
	  const uint e1, const uint e2, const double rkdt);

	// Update fields with residues.
	void
	 updateFieldsWithRes(
	  const uint e1,
	  const uint e2,
	  const double rkb);
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

	void
	LTSSaveFieldsAndResidues(
			const uint fKSave,
			const uint lKSave);
	void
	LTSLoadFieldsAndResidues(
			const uint fKSave,
			const uint lKSave);

private:
	void
 	 assignMatrices(const CellGroup& cells);
	void
	 buildCMatrices(const CellGroup& cell);
	virtual void
	 buildMaterials(
	  const CellGroup& cells,
	  const ArgumentsDGTD* arg);

};

#endif /* SOLVERLM_H_ */
