/*
 * SolverExplicit.h
 *
 *  Created on: Nov 30, 2012
 *      Author: luis
 */

#ifndef SOLVEREXPLICIT_H_

#include "DG.h"
#include "DGCurvedFace.h"

class DGExplicit : public DG {
    friend class IntegratorLSERK;
    friend class IntegratorLF2;
    friend class IntegratorLF2Full;
    friend class IntegratorVerlet;
public:
    DGExplicit(const SmbData* smb, Comm* comm);
    virtual ~DGExplicit();
    UInt getFieldDOFs();
    const FieldR3& getRHSElectric() const;
    const FieldR3& getRHSMagnetic() const;
    void printInfo() const;
protected:
    void computeRHS(
            const UInt e1,
            const UInt e2,
            const Real localtime,
            const Real rkdt);
    void computeRHSElectric(
            const UInt e1,
            const UInt e2,
            const Real localtime,
            const Real minDT);
    void computeRHSMagnetic(
            const UInt e1,
            const UInt e2,
            const Real localtime,
            const Real minDT);
    void computeCurlsInRHSElectric(const UInt e1, const UInt e2);
    void computeCurlsInRHSMagnetic(const UInt e1, const UInt e2);
    void computeJumps(
            const UInt e1,
            const UInt e2,
            const Real localTime,
            const Real minDT);
    void copyJumpsToResidueJumps(
            const UInt e1,
            const UInt e2);
    void addFluxesToRHSElectric(
            const UInt e1, const UInt e2);
    void addFluxesToRHSMagnetic(
            const UInt e1, const UInt e2);
    void addFluxesToRHSElectric(
            const UInt e1, const UInt e2, const bool useResForUpw);
    void addFluxesToRHSMagnetic(
            const UInt e1, const UInt e2, const bool useResForUpw);
    void addStraightFluxesToRHSElectric(const UInt e1, const UInt e2,
            const bool useResForUpw);
    void addStraightFluxesToRHSMagnetic(const UInt e1, const UInt e2,
            const bool useResForUpw);
    void addCurvedFluxesToRHSElectric(const UInt e1, const UInt e2,
            const bool useResForUpw);
    void addCurvedFluxesToRHSMagnetic(const UInt e1, const UInt e2,
            const bool useResForUpw);
    void computePolarizationCurrentsRHS(
            const UInt e1, const UInt e2);
    void computePolarizationCurrentsRHSElectric(
            const UInt e1, const UInt e2);
    void computePolarizationCurrentsRHSMagnetic(
            const UInt e1, const UInt e2);
    void addRHSToFieldsElectric(
            const UInt e1,
            const UInt e2,
            const Real rkdt);
    void addRHSToFieldsMagnetic(
            const UInt e1,
            const UInt e2,
            const Real rkdt);
    UInt
    getIndexOfElement(const UInt e) const {
        return (e * np);
    }
    void addRHSToResidueElectric(const UInt e1, const UInt e2,
            const Real rkdt);
    void addRHSToResidueMagnetic(const UInt e1, const UInt e2,
            const Real rkdt);
    void addRHSToRes(
            const UInt e1,
            const UInt e2,
            const Real rka,
            const Real dt);
    void updateFieldsWithRes(
            const UInt e1,
            const UInt e2,
            const Real rkb);
    void LTSSaveFieldsAndResidues(
            const UInt fKSave,
            const UInt lKSave);
    void LTSLoadFieldsAndResidues(
            const UInt fKSave,
            const UInt lKSave);
private:
    // - Maps.
    Int vmapM[faces][nfp];
    Int vmapP[16][nfp];
    Int ***map;
    // Pointers to neighbour fields. dim = (nK, 4).
    Real ***ExP, ***EyP, ***EzP, ***HxP, ***HyP, ***HzP;
    // Curved faces stuff ---------------------------------------------
    UInt nCurvedFaces;
    DGCurvedFace *curveFace;
    const Real **Cx, **Cy, **Cz; // Pointers to C. dim = (nK)
    // Fields and residuals: dim = (np,nK)
    FieldR3 rhsE, rhsH;
    FieldR3 savedResE, savedResH;
    FieldR3 savedE, savedH;
    FieldR3 nE, nH;
    // Jumps and fluxes: dim = (4*nfp, nK)
    FieldR3 dE, dH;
    FieldR3 dresE, dresH;
    // BC lists. nSMA, nPEC and nPMC are the number of BC of each kind.
    // BC are stored as pointers to memory positions in the jumps.
    // dim = (nK)
    UInt nSMA, nPEC, nPMC;
    UInt *SMAe, *SMAf, *PECe, *PECf, *PMCe, *PMCf;
    vector<DGSource*> source;
    vector<DGDispersive*> dispersive;
    void buildMaterials(const OptionsSolverDGTD* arg);
    void deduplicateVMaps();
    void allocateRHSAndJumps();
    void allocateMaps();
    void assignPointersToNeighbours(const MeshVolume& mesh);
    void buildScalingFactors(const MapGroup& map);
    void buildEMSources(
            const EMSourceGroup& emSource,
            const MapGroup& maps);
    bool checkPtrsToNeigh() const;
    void assignMatrices();
    void allocateFieldsForLTS();
    void buildCurvedFluxScalingFactors(const MapGroup& map);
};

#endif /* SOLVEREXPLICIT_H_ */
