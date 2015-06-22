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
            Comm* comm,
            const OptionsSolverDGTD* arg);
    virtual ~DGLowMemory();
    void printInfo() const;
protected:
    // Computes residue
    void computeCurlsInRHSElectric(const UInt e1, const UInt e2);
    void computeCurlsInRHSMagnetic(const UInt e1, const UInt e2);
    void computeJumps(
            const UInt e1,
            const UInt e2,
            const Real localTime,
            const Real minDT);
    void addFluxesToRHSElectric(
            const UInt e1, const UInt e2);
    void addFluxesToRHSMagnetic(
            const UInt e1, const UInt e2);
    void computePolarizationCurrentsRHS(
            const UInt e1, const UInt e2);
    void computePolarizationCurrentsRHSElectric(
            const UInt e1, const UInt e2);
    void computePolarizationCurrentsRHSMagnetic(
            const UInt e1, const UInt e2);
    void addRHSToRes(
            const UInt e1,
            const UInt e2,
            const Real rka,
            const Real dt);
    void addRHSToResidueElectric(
            const UInt e1, const UInt e2, const Real rkdt);
    void addRHSToResidueMagnetic(
            const UInt e1, const UInt e2, const Real rkdt);

    // Update fields with residues.
    void updateFieldsWithRes(
            const UInt e1,
            const UInt e2,
            const Real rkb);
    void addRHSToFieldsElectric(
            const UInt e1,
            const UInt e2,
            const Real rkdt);
    void addRHSToFieldsMagnetic(
            const UInt e1,
            const UInt e2,
            const Real rkdt);

    void LTSSaveFieldsAndResidues(
            const UInt fKSave,
            const UInt lKSave);
    void LTSLoadFieldsAndResidues(
            const UInt fKSave,
            const UInt lKSave);

private:
    void assignMatrices(const CellGroup& cells);
    void buildCMatrices(const CellGroup& cell);
    virtual void buildMaterials(
            const CellGroup& cells,
            const OptionsSolverDGTD* arg);

};

#endif /* SOLVERLM_H_ */
