/*
 * SolverExplicit.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: luis
 */
#include "DGExplicit.h"


DGExplicit::DGExplicit(
 const SmbData* smb,
 Comm* comm_,
 const ArgumentsCudg3d* arg) {
   smb_ = smb;
   CellGroup cells(smb);
   BCGroup bc(smb);
	init(arg, smb->pMGroup, &cells, comm_);
	allocateRHSAndJumps(arg);
	if (!arg->isNoLTS()) {
		allocateFieldsForLTS();
	}
	if (smb->emSources->count() != 0) {
		setFieldsToZero();
	} else {
		setFieldsToRandom();
		cout<< ">> No EM Excitations were detected <<" << endl;
		cout<< ">> A random field is being used  <<" << endl;
	}
	allocateMaps();
	deduplicateVMaps(cells);
	assignPointersToNeighbours(cells, *smb->mesh);
	buildEMSources(*smb->emSources, bc, smb->mesh->map, cells);
	BCGroupsToLocalArray(bc, cells, smb->mesh->map);
	buildScalingFactors(cells, smb->mesh->map);
}

DGExplicit::~DGExplicit() {

}

uint
DGExplicit::getFieldDOFs() {
	return (nK * np * 3);
}

const FieldR3&
DGExplicit::getRHSElectric() const {
	return rhsE;
}

const FieldR3&
DGExplicit::getRHSMagnetic() const {
	return rhsH;
}

void
DGExplicit:: printInfo() const {
	cout<< " --- SolverExplicit info ---" << endl;
	cout<< "- Boundary conditions: " << endl;
	cout<< "# SMA: " << nSMA << endl;
	cout<< "# PEC: " << nPEC << endl;
	cout<< "# PMC: " << nPMC << endl;
	cout<< "# Of Curved Faces: " << nCurvedFaces << endl;
}

void
DGExplicit::computePolarizationCurrentsRHS(
 const uint e1,
 const uint e2) {
	computePolarizationCurrentsRHSElectric(e1,e2);
	computePolarizationCurrentsRHSMagnetic(e1,e2);
}

void
DGExplicit::computePolarizationCurrentsRHSElectric(
 const uint e1, const uint e2) {
	for (uint d = 0; d < dispersive.size(); d++) {
		dispersive[d]->computeRHSElectricPolarizationCurrents(E, e1, e2);
		dispersive[d]->computeRHSElectric(rhsE, E, e1, e2);
	}
}

void
DGExplicit::computePolarizationCurrentsRHSMagnetic(
 const uint e1, const uint e2) {
	for (uint d = 0; d < dispersive.size(); d++) {
		dispersive[d]->computeRHSMagneticPolarizationCurrents(H, e1, e2);
		dispersive[d]->computeRHSMagnetic(rhsH, H, e1, e2);
	}
}

void
DGExplicit::computeRHS(
 const uint e1,
 const uint e2,
 const double localTime,
 const double minDT) {
	computeRHSElectric(e1,e2, localTime,minDT);
	computeRHSMagnetic(e1,e2, localTime,minDT);
}

void
DGExplicit::computeRHSElectric(
 const uint e1,
 const uint e2,
 const double localTime,
 const double minDT) {
	computeCurlsInRHSElectric(e1,e2);
	computeJumps(e1,e2, localTime,minDT);
	addFluxesToRHSElectric(e1,e2);
	uint i, j, e;
#	ifdef SOLVER_USE_OPENMP
#	pragma omp parallel for private(i,j,e)
#	endif
	for (e = e1; e < e2; e++) {
		i = e * np;
		j = (e + 1) * np;
		rhsE.prod(i, j, oneOverEps[e]);
	}
}

void
DGExplicit::computeRHSMagnetic(
 const uint e1,
 const uint e2,
 const double localTime,
 const double minDT) {
	computeCurlsInRHSMagnetic(e1,e2);
	computeJumps(e1,e2,localTime,minDT);
	addFluxesToRHSMagnetic(e1,e2);
	uint i, j, e;
#	ifdef SOLVER_USE_OPENMP
#	pragma omp parallel for private(i,j,e)
#	endif
	for (e = e1; e < e2; e++) {
		i = e * np;
		j = (e + 1) * np;
		rhsH.prod(i, j, oneOverMu[e]);
	}
}

void
DGExplicit::computeCurlsInRHSElectric(
 const uint e1,
 const uint e2) {
	uint i, e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(e,i)
	#endif
	for (e = e1; e < e2; e++) {
		// i: Beginning of element field. [0, (nK-1)*np]
		i = e * np;
		// rhsE(x) = + Cy * H(z) - Cz * H(y)
		m_v_prod<double,np,np>(&rhsE.set(x)[i], Cy[e], &H(z)[i]);
		sub_m_v_prod<double,np,np>(&rhsE.set(x)[i], Cz[e], &H(y)[i]);
		// rhsE(y) = + Cz * H(x) - Cx * H(z)
		m_v_prod<double,np,np>(&rhsE.set(y)[i], Cz[e], &H(x)[i]);
		sub_m_v_prod<double,np,np>(&rhsE.set(y)[i], Cx[e], &H(z)[i]);
		// rhsE(z) = + Cx * H(y) - Cy * H(x)
		m_v_prod<double,np,np>(&rhsE.set(z)[i], Cx[e], &H(y)[i]);
		sub_m_v_prod<double,np,np>(&rhsE.set(z)[i], Cy[e], &H(x)[i]);
	}
}



void
DGExplicit::computeCurlsInRHSMagnetic(
 const uint e1,
 const uint e2) {
	uint i, e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(e,i)
	#endif
	for (e = e1; e < e2; e++) {
		// i: Beginning of element field. [0, (nK-1)*np]
		i = e * np;
		// rhsH(x) = - Cy * E(z) + Cz * E(y)
		am_v_prod<double,np,np>(&rhsH.set(x)[i], Cy[e], &E(z)[i], -1.0);
		add_m_v_prod<double,np,np>(&rhsH.set(x)[i], Cz[e], &E(y)[i]);
		// rhsH(y) = - Cz * E(x) + Cx * E(z)
		am_v_prod<double,np,np>(&rhsH.set(y)[i], Cz[e], &E(x)[i], -1.0);
		add_m_v_prod<double,np,np>(&rhsH.set(y)[i], Cx[e], &E(z)[i]);
		// rhsH(z) = - Cx * E(y) + Cy * E(x)
		am_v_prod<double,np,np>(&rhsH.set(z)[i], Cx[e], &E(y)[i], -1.0);
		add_m_v_prod<double,np,np>(&rhsH.set(z)[i], Cy[e], &E(x)[i]);
	}
}
void
DGExplicit::computeJumps(
 const uint e1,
 const uint e2,
 const double localTime,
 const double minDT) {
	if (comm->getNumberOfTasks() > 1) {
		comm->syncNeighbourFields(
		 nE.set(x), nE.set(y), nE.set(z), nH.set(x), nH.set(y), nH.set(z),
		 E(x),E(y),E(z),H(x),H(y),H(z));
	}
	uint b, i, j, k, f, e;
	uint vM, vP;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(e,k,i,f,j,vM,vP)
	#endif
	for (e = e1; e < e2; e++) {
		k = e * np;   // Beginning of element field. [0, (nK-1)*np]
		i = e * nfp * faces;
		for (uint f = 0; f < faces; f++) {
			for (uint j = 0; j < nfp; j++) {
				vM = k + vmapM[f][j]; // Local field pos.
				vP = map[e][f][j]; // Neigh field pos.
				dE.set(x)[i] = E(x)[vM] - ExP[e][f][vP];
				dE.set(y)[i] = E(y)[vM] - EyP[e][f][vP];
				dE.set(z)[i] = E(z)[vM] - EzP[e][f][vP];
				dH.set(x)[i] = H(x)[vM] - HxP[e][f][vP];
				dH.set(y)[i] = H(y)[vM] - HyP[e][f][vP];
				dH.set(z)[i] = H(z)[vM] - HzP[e][f][vP];
				i++;
			}
		}
	}
	// SMA
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(b,e,k,i,f,j,vM,vP)
	#endif
	for (b = 0; b < nSMA; b++) {
		f = SMAf[b];
		e = SMAe[b];
		if (e >= e1 && e < e2) {
			i = e * nfp * faces + f * nfp;
			k = e * np;
			for (j = 0; j < nfp; j++) {
				vM = vmapM[f][j];
				dE.set(x)[i + j] = E(x)[k + vM];
				dE.set(y)[i + j] = E(y)[k + vM];
				dE.set(z)[i + j] = E(z)[k + vM];
				dH.set(x)[i + j] = H(x)[k + vM];
				dH.set(y)[i + j] = H(y)[k + vM];
				dH.set(z)[i + j] = H(z)[k + vM];
			}
		}
	}
	// PEC
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(b,e,k,i,f,j,vM,vP)
	#endif
	for (b = 0; b < nPEC; b++) {
		f = PECf[b];
		e = PECe[b];
		if (e >= e1 && e < e2) {
			i = e * nfp * faces + f * nfp;
			k = e * np;
			for (j = 0; j < nfp; j++) {
				vM = vmapM[f][j];
				dE.set(x)[i + j] = 2.0 * E(x)[k + vM];
				dE.set(y)[i + j] = 2.0 * E(y)[k + vM];
				dE.set(z)[i + j] = 2.0 * E(z)[k + vM];
				dH.set(x)[i + j] = 0.0;
				dH.set(y)[i + j] = 0.0;
				dH.set(z)[i + j] = 0.0;
			}
		}
	}
	// PMC
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(b,e,k,i,f,j,vM,vP)
	#endif
	for (b = 0; b < nPMC; b++) {
		f = PMCf[b];
		e = PMCe[b];
		if (e >= e1 && e < e2) {
			i = e * nfp * faces + f * nfp;
			k = e * np;
			for (j = 0; j < nfp; j++) {
				vM = vmapM[f][j];
				dE.set(x)[i + j] = 0.0;
				dE.set(y)[i + j] = 0.0;
				dE.set(z)[i + j] = 0.0;
				dH.set(x)[i + j] = 2.0 * H(x)[k + vM];
				dH.set(y)[i + j] = 2.0 * H(y)[k + vM];
				dH.set(z)[i + j] = 2.0 * H(z)[k + vM];
			}
		}
	}
	// Computes E(x)citations.
	for (b = 0; b < source.size(); b++) {
		source[b]->computeExcitation(localTime,minDT);
		source[b]->addJumps(e1,e2);
	}
	// Computes contributions of polarization currents to jumps
	for (b = 0; b < dispersive.size(); b++) {
		dispersive[b]->addJumps(dE, dH, E, H, e1,e2);
	}
}

void
DGExplicit::addFluxesToRHSElectric(
 const uint e1,
 const uint e2) {
	static const bool useResForUpw = false;
	addFluxesToRHSElectric(e1,e2,useResForUpw);
}

void
DGExplicit::addFluxesToRHSMagnetic(
 const uint e1,
 const uint e2) {
	static const bool useResForUpw = false;
	addFluxesToRHSMagnetic(e1,e2,useResForUpw);
}

void
DGExplicit::addFluxesToRHSElectric(
 const uint e1,
 const uint e2,
 const bool useResForUpw) {
	addStraightFluxesToRHSElectric(e1,e2,useResForUpw);
	addCurvedFluxesToRHSElectric(e1,e2,useResForUpw);
}

void
DGExplicit::addFluxesToRHSMagnetic(
 const uint e1,
 const uint e2,
 const bool useResForUpw) {
	addStraightFluxesToRHSMagnetic(e1,e2,useResForUpw);
	addCurvedFluxesToRHSMagnetic(e1,e2,useResForUpw);
}

void
DGExplicit::addStraightFluxesToRHSElectric(
 const uint e1,
 const uint e2,
 const bool useResForUpw) {
	uint i,j, k, f, e;
	double fx[nfpfaces], fy[nfpfaces], fz[nfpfaces];
	if (upwinding == 0.0) {
		// ---------- Centred flux ------------------------------------
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
		#endif
		for (e = e1; e < e2; e++) {
			i = e * nfpfaces;
			for (k = 0; k < nfpfaces; k++) {
				f = k / nfp;
				j = e * faces + f;
				// - n ^ dH ---------------------------------------------------
				fx[k] = - nImp(y)[j] * dH(z)[i] + nImp(z)[j] * dH(y)[i];
				fy[k] = - nImp(z)[j] * dH(x)[i] + nImp(x)[j] * dH(z)[i];
				fz[k] = - nImp(x)[j] * dH(y)[i] + nImp(y)[j] * dH(x)[i];
				i++;
			}
			i = e * np;
			add_m_v_prod<double,np,nfpfaces>(&rhsE.set(x)[i], LIFT, fx);
			add_m_v_prod<double,np,nfpfaces>(&rhsE.set(y)[i], LIFT, fy);
			add_m_v_prod<double,np,nfpfaces>(&rhsE.set(z)[i], LIFT, fz);
		}
	} else if (upwinding == 1.0) {
		// ---------- Upwind flux -------------------------------------
		if (useResForUpw) {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = - nImp(y)[j]  * dH(z)[i] + nImp(z)[j]  * dH(y)[i]
							+ cnImp(x)[j] * dresE(y)[i] + cnImp(z)[j] * dresE(z)[i]
							- rnImp(x)[j] * dresE(x)[i];
					fy[k] = - nImp(z)[j]  * dH(x)[i] + nImp(x)[j]  * dH(z)[i]
							 + cnImp(y)[j] * dresE(z)[i] + cnImp(x)[j] * dresE(x)[i]
							 - rnImp(y)[j] * dresE(y)[i];
					fz[k] = - nImp(x)[j]  * dH(y)[i] + nImp(y)[j]  * dH(x)[i]
							 + cnImp(z)[j] * dresE(x)[i] + cnImp(y)[j] * dresE(y)[i]
							 - rnImp(z)[j] * dresE(z)[i];
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(z)[i], LIFT, fz);
			}
		} else {
			// >>>>>>> This one is the most frequently used <<<<<<<<<
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = - nImp(y)[j]  * dH(z)[i] + nImp(z)[j]  * dH(y)[i]
							 + cnImp(x)[j] * dE(y)[i] + cnImp(z)[j] * dE(z)[i]
							 - rnImp(x)[j] * dE(x)[i];
					fy[k] = - nImp(z)[j]  * dH(x)[i] + nImp(x)[j]  * dH(z)[i]
							 + cnImp(y)[j] * dE(z)[i] + cnImp(x)[j] * dE(x)[i]
							 - rnImp(y)[j] * dE(y)[i];
					fz[k] = - nImp(x)[j]  * dH(y)[i] + nImp(y)[j]  * dH(x)[i]
							 + cnImp(z)[j] * dE(x)[i] + cnImp(y)[j] * dE(y)[i]
							 - rnImp(z)[j] * dE(z)[i];
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(z)[i], LIFT, fz);
			}
			// >>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
		}
	} else {
		// ---------- Penalized flux ----------------------------------
		if (useResForUpw) {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = - nImp(y)[j]  * dH(z)[i] + nImp(z)[j]  * dH(y)[i]
					 + (cnImp(x)[j] * dresE(y)[i] + cnImp(z)[j] * dresE(z)[i]
					 - rnImp(x)[j] * dresE(x)[i]) * upwinding;
					fy[k] = - nImp(z)[j]  * dH(x)[i] + nImp(x)[j]  * dH(z)[i]
					 + (cnImp(y)[j] * dresE(z)[i] + cnImp(x)[j] * dresE(x)[i]
					 - rnImp(y)[j] * dresE(y)[i]) * upwinding;
					fz[k] = - nImp(x)[j]  * dH(y)[i] + nImp(y)[j]  * dH(x)[i]
					 + (cnImp(z)[j] * dresE(x)[i] + cnImp(y)[j] * dresE(y)[i]
					 - rnImp(z)[j] * dresE(z)[i]) * upwinding;
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(z)[i], LIFT, fz);
			}
		} else {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = - nImp(y)[j]  * dH(z)[i] + nImp(z)[j]  * dH(y)[i]
					 + (cnImp(x)[j] * dE(y)[i] + cnImp(z)[j] * dE(z)[i]
					 - rnImp(x)[j] * dE(x)[i]) * upwinding;
					fy[k] = - nImp(z)[j]  * dH(x)[i] + nImp(x)[j]  * dH(z)[i]
					 + (cnImp(y)[j] * dE(z)[i] + cnImp(x)[j] * dE(x)[i]
					 - rnImp(y)[j] * dE(y)[i]) * upwinding;
					fz[k] = - nImp(x)[j]  * dH(y)[i] + nImp(y)[j]  * dH(x)[i]
					 + (cnImp(z)[j] * dE(x)[i] + cnImp(y)[j] * dE(y)[i]
					 - rnImp(z)[j] * dE(z)[i]) * upwinding;
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(x)[i],LIFT,fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(y)[i],LIFT,fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsE.set(z)[i],LIFT,fz);
			}
		}
	}
}

void
DGExplicit::addStraightFluxesToRHSMagnetic (
 const uint e1,
 const uint e2,
 const bool useResForUpw) {
	uint i, j, f, e, k;
	double fx[nfpfaces], fy[nfpfaces], fz[nfpfaces];
	if (upwinding == 0.0) {
		// ---------- Centred flux --------------------------------------------
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
		#endif
		for (e = e1; e < e2; e++) {
			i = e * nfpfaces;
			for (k = 0; k < nfpfaces; k++) {
				f = k / nfp;
				j = e * faces + f;
				fx[k] = + nAdm(y)[j] * dE(z)[i] - nAdm(z)[j]  * dE(y)[i];
				fy[k] = + nAdm(z)[j] * dE(x)[i] - nAdm(x)[j]  * dE(z)[i];
				fz[k] = + nAdm(x)[j] * dE(y)[i] - nAdm(y)[j]  * dE(x)[i];
				i++;
			}
			i = e * np;
			add_m_v_prod<double,np,nfpfaces>(&rhsH.set(x)[i], LIFT, fx);
			add_m_v_prod<double,np,nfpfaces>(&rhsH.set(y)[i], LIFT, fy);
			add_m_v_prod<double,np,nfpfaces>(&rhsH.set(z)[i], LIFT, fz);
		}
	} else if (upwinding == 1.0) {
		// ---------- Upwind flux ---------------------------------------------
		if (useResForUpw) {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = + nAdm(y)[j]  * dE(z)[i] - nAdm(z)[j]  * dE(y)[i]
					 + cnAdm(x)[j] * dresH(y)[i] + cnAdm(z)[j] * dresH(z)[i]
					 - rnAdm(x)[j] * dresH(x)[i];
					fy[k] = + nAdm(z)[j]  * dE(x)[i] - nAdm(x)[j]  * dE(z)[i]
					 + cnAdm(y)[j] * dresH(z)[i] + cnAdm(x)[j] * dresH(x)[i]
					 - rnAdm(y)[j] * dresH(y)[i];
					fz[k] = + nAdm(x)[j]  * dE(y)[i]  - nAdm(y)[j] * dE(x)[i]
					 + cnAdm(z)[j] * dresH(x)[i]  + cnAdm(y)[j] * dresH(y)[i]
					 - rnAdm(z)[j] * dresH(z)[i];
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(z)[i], LIFT, fz);
			}
		} else {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = + nAdm(y)[j]  * dE(z)[i] - nAdm(z)[j]  * dE(y)[i]
					 + cnAdm(x)[j] * dH(y)[i] + cnAdm(z)[j] * dH(z)[i]
					 - rnAdm(x)[j] * dH(x)[i];
					fy[k] = + nAdm(z)[j]  * dE(x)[i] - nAdm(x)[j]  * dE(z)[i]
					 + cnAdm(y)[j] * dH(z)[i] + cnAdm(x)[j] * dH(x)[i]
					 - rnAdm(y)[j] * dH(y)[i];
					fz[k] = + nAdm(x)[j]  * dE(y)[i]  - nAdm(y)[j]  * dE(x)[i]
					 + cnAdm(z)[j] * dH(x)[i]  + cnAdm(y)[j] * dH(y)[i]
					 - rnAdm(z)[j] * dH(z)[i];
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(z)[i], LIFT, fz);
			}
		}
	} else {
		// ---------- Penalized flux ------------------------------------------
		if (useResForUpw) {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = + nAdm(y)[j]  * dE(z)[i] - nAdm(z)[j]  * dE(y)[i]
					 + (cnAdm(x)[j] * dresH(y)[i] + cnAdm(z)[j] * dresH(z)[i]
					 - rnAdm(x)[j] * dresH(x)[i]) * upwinding;
					fy[k] = + nAdm(z)[j]  * dE(x)[i] - nAdm(x)[j]  * dE(z)[i]
					 + (cnAdm(y)[j] * dresH(z)[i] + cnAdm(x)[j] * dresH(x)[i]
					 - rnAdm(y)[j] * dresH(y)[i]) * upwinding;
					fz[k] = + nAdm(x)[j]  * dE(y)[i]  - nAdm(y)[j]  * dE(x)[i]
					 + (cnAdm(z)[j] * dresH(x)[i]  + cnAdm(y)[j] * dresH(y)[i]
					 - rnAdm(z)[j] * dresH(z)[i]) * upwinding;
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(z)[i], LIFT, fz);
			}
		} else {
			#ifdef SOLVER_USE_OPENMP
			#pragma omp parallel for private(i,j,k,e,f,fx,fy,fz)
			#endif
			for (e = e1; e < e2; e++) {
				i = e * nfpfaces;
				for (k = 0; k < nfpfaces; k++) {
					f = k / nfp;
					j = e * faces + f;
					fx[k] = + nAdm(y)[j]  * dE(z)[i] - nAdm(z)[j]  * dE(y)[i]
					 + (cnAdm(x)[j] * dH(y)[i] + cnAdm(z)[j] * dH(z)[i]
					 - rnAdm(x)[j] * dH(x)[i]) * upwinding;
					fy[k] = + nAdm(z)[j]  * dE(x)[i] - nAdm(x)[j]  * dE(z)[i]
					 + (cnAdm(y)[j] * dH(z)[i] + cnAdm(x)[j] * dH(x)[i]
					 - rnAdm(y)[j] * dH(y)[i]) * upwinding;
					fz[k] = + nAdm(x)[j]  * dE(y)[i]  - nAdm(y)[j]  * dE(x)[i]
					 + (cnAdm(z)[j] * dH(x)[i]  + cnAdm(y)[j] * dH(y)[i]
					 - rnAdm(z)[j] * dH(z)[i]) * upwinding;
					i++;
				}
				i = e * np;
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(x)[i], LIFT, fx);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(y)[i], LIFT, fy);
				add_m_v_prod<double,np,nfpfaces>(&rhsH.set(z)[i], LIFT, fz);
			}
		}
	}
}
void
DGExplicit::addCurvedFluxesToRHSElectric(
 const uint e1,
 const uint e2,
 const bool useResForUpw) {
#	ifndef SOLVERExplicit_IGNORE_CURVED_FLUXES
	uint c;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(c)
	#endif
	for (c = 0; c < nCurvedFaces; c++) {
		if (e1 <= curveFace[c].solverPosition
		 && curveFace[c].solverPosition < e2) {
			curveFace[c].addFluxToRHSElectric(upwinding, useResForUpw);
		}
	}
#	endif
}

void
DGExplicit::addCurvedFluxesToRHSMagnetic(
 const uint e1,
 const uint e2,
 const bool useResForUpw) {
#	ifndef SOLVERExplicit_IGNORE_CURVED_FLUXES
	uint c;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(c)
	#endif
	for (c = 0; c < nCurvedFaces; c++) {
		if (e1 <= curveFace[c].solverPosition
		 && curveFace[c].solverPosition < e2) {
			curveFace[c].addFluxToRHSMagnetic(upwinding, useResForUpw);
		}
	}
#	endif
}

void
DGExplicit::addRHSToFieldsElectric(
 const uint e1,
 const uint e2,
 const double rkdt) {
	uint init = getIndexOfElement(e1);
	uint end = getIndexOfElement(e2);
	E.addProd_omp(init, end, getRHSElectric(), rkdt);
}

void
DGExplicit::addRHSToFieldsMagnetic(
 const uint e1,
 const uint e2,
 const double rkdt) {
	uint init = getIndexOfElement(e1);
	uint end = getIndexOfElement(e2);
	H.addProd_omp(init, end, getRHSMagnetic(), rkdt);
}

void
DGExplicit::allocateRHSAndJumps(
 const ArgumentsCudg3d* args) {
	rhsE.setSize(getFieldDOFs()/3);
	rhsH.setSize(getFieldDOFs()/3);
	dE.setSize(nK*nfp*4);
	dH.setSize(nK*nfp*4);
}

void
DGExplicit::allocateMaps() {
	ExP = new double**[nK];
	EyP = new double**[nK];
	EzP = new double**[nK];
	HxP = new double**[nK];
	HyP = new double**[nK];
	HzP = new double**[nK];
	for (uint e = 0; e < nK; e++) {
		ExP[e] = new double*[faces];
		EyP[e] = new double*[faces];
		EzP[e] = new double*[faces];
		HxP[e] = new double*[faces];
		HyP[e] = new double*[faces];
		HzP[e] = new double*[faces];
		for (uint f = 0; f < faces; f++) {
			ExP[e][f] = NULL;
			EyP[e][f] = NULL;
			EzP[e][f] = NULL;
			HxP[e][f] = NULL;
			HyP[e][f] = NULL;
			HzP[e][f] = NULL;
		}
	}
	map = new int**[nK];
	for (uint e = 0; e < nK; e++) {
		map[e] = new int*[faces];
		for (uint f = 0; f < faces; f++) {
			map[e][f] = NULL;
		}
	}
}

void
DGExplicit::assignMatrices(const CellGroup& cells) {
	// Assigns matrices.
	Cx = new const double*[nK];
	Cy = new const double*[nK];
	Cz = new const double*[nK];
	uint e;
#	ifdef SOLVER_DEDUPLICATE_OPERATORS
#	ifdef SOLVER_USE_OPENMP
#	pragma omp parallel for private(e)
#	endif
	for (e = 0; e < nK; e++) {
		StaMatrix<double,np,np> C[3];
		cells.getPtrToCellWithId(cells.getIdOfRelPos(e))->getCMatrices(C);
		for (uint i = 0; i < 3; i++) {
			set<StaMatrix<double,np,np>, lexCompareMat>::iterator it;
			it = CList.find(C[i]);
			if (it != CList.end()) {
				if (i == 0) {
					Cx[e] = it->val();
				}
				if (i == 1) {
					Cy[e] = it->val();
				}
				if (i == 2) {
					Cz[e] = it->val();
				}
			} else {
				cerr << endl << "ERROR @ SolverExplicit:"
					<< "The following matrix is not stored." << endl;
				C[i].printInfo();
			}
		}
	}
 	cout<< "Matrix deduplication: " << nK*3
 		<< " --> " << CList.size() << endl;
#	else
 	CList = new StaMatrix<double,np,np>[3*nK];
#	ifdef SOLVER_USE_OPENMP
#	pragma omp parallel for private(e)
#	endif
	for (e = 0; e < nK; e++) {
		uint id = cells.getIdOfRelPos(e);
		const CellTet<ORDER_N>* cell_ = cells.getPtrToCellWithId(id);
		for (uint i = 0; i < 3; i++) {
			uint j = 3 * e + i;
			if (i == 0) {
				Cx[e] = CList[j].val();
			}
			if (i == 1) {
				Cy[e] = CList[j].val();
			}
			if (i == 2) {
				Cz[e] = CList[j].val();
			}
		}
	}
#	endif
}

void
DGExplicit::assignPointersToNeighbours(
 const CellGroup& cells,
 const MeshVolume& mesh) {
	uint nNeighs = 0;
	for (uint k = 0; k < nK; k++) {
		uint id1 = cells.getIdOfRelPos(k);
		for (uint f = 0; f < faces; f++) {
			uint id2 = mesh.map.getNeighbour(id1,f)->getId();
			if (cells.isLocalId(id2)) {
				// Assigns ptrs to local cells and counts non local neigh.
				uint e2 = cells.getRelPosOfId(id2);
				ExP[k][f] = &E.set(x)[e2 * np];
				EyP[k][f] = &E.set(y)[e2 * np];
				EzP[k][f] = &E.set(z)[e2 * np];
				HxP[k][f] = &H.set(x)[e2 * np];
				HyP[k][f] = &H.set(y)[e2 * np];
				HzP[k][f] = &H.set(z)[e2 * np];
			} else {
				nNeighs++;
			}
		}
	}
	// Stores neighbours information and allocates fields.
	vector<uint> neighId;
	neighId.reserve(nNeighs);
	nE.setSize(nNeighs*np);
	nH.setSize(nNeighs*np);
	nE.setToZero();
	nH.setToZero();
	uint neigh = 0;
	for (uint k = 0; k < nK; k++) {
		uint id1 = cells.getIdOfRelPos(k);
		for (uint f = 0; f < faces; f++) {
			assert(cells.isLocalId(id1));
			uint id2 = mesh.map.getNeighbour(id1,f)->getId();
			if (!cells.isLocalId(id2)) {
				neighId.push_back(id2);
				ExP[k][f] = &nE.set(x)[neigh * np];
				EyP[k][f] = &nE.set(y)[neigh * np];
				EzP[k][f] = &nE.set(z)[neigh * np];
				HxP[k][f] = &nH.set(x)[neigh * np];
				HyP[k][f] = &nH.set(y)[neigh * np];
				HzP[k][f] = &nH.set(z)[neigh * np];
				neigh++;
			}
		}
	}
	//
	comm->initNeighbourFields(neighId);
	assert(neigh == nNeighs);
	assert(checkPtrsToNeigh());
}

void
DGExplicit::BCGroupsToLocalArray(
 const BCGroup& bc,
 const CellGroup& cells,
 const MapGroup& maps) {
	// ----------- SMA ------------------------------------------------
   // Counts SMAs and allocates, em boundaries are also considered.
   {
      vector<const BoundaryCondition*> smaPtr = bc.get(Condition::sma);
      vector<const BoundaryCondition*> emPtr = bc.get(Condition::emSource);
      // Removes non local elements.
      smaPtr = removeNonLocalBCs(&cells, smaPtr);
      emPtr = removeNonLocalBCs(&cells, emPtr);
      // Stores em sources at boundaries.
      vector<const BoundaryCondition*> emAtDomainBound;
      for (uint i = 0; i < emPtr.size(); i++) {
         if (maps.isDomainBoundary(emPtr[i]->get())) {
            emAtDomainBound.push_back(emPtr[i]);
         }
      }
      nSMA = smaPtr.size() + emAtDomainBound.size();
      SMAe = new uint[nSMA];
      SMAf = new uint[nSMA];
      // Stores solver relative positions and faces.
      uint j = 0;
      for (uint i = 0; i < smaPtr.size(); i++) {
         SMAe[j] = cells.getRelPosOfId(smaPtr[i]->getCell()->getId());
         SMAf[j] = smaPtr[i]->getFace();
         j++;
      }
      for (uint i = 0; i < emAtDomainBound.size(); i++) {
         SMAe[j] = cells.getRelPosOfId(emAtDomainBound[i]->getCell()->getId());
         SMAf[j] = emAtDomainBound[i]->getFace();
         j++;
      }
      assert(nSMA == j);
   }
	// ----------- PEC ------------------------------------------------
	// Counts and allocates.
	{
	   vector<const BoundaryCondition*> pecPtr = bc.get(Condition::pec);
	   pecPtr = removeNonLocalBCs(&cells, pecPtr);
	   nPEC = pecPtr.size();
	   PECe = new uint[nPEC];
	   PECf = new uint[nPEC];
	   // Stores solver rel pos and faces.
	   for (uint i = 0; i < pecPtr.size(); i++) {
	      PECe[i] = cells.getRelPosOfId(pecPtr[i]->getCell()->getId());
	      PECf[i] = pecPtr[i]->getFace();
	   }
	}
	// ----------- PMC ------------------------------------------------
	// Counts and allocates.
	{
	   vector<const BoundaryCondition*> pmcPtr = bc.get(Condition::pmc);
	   pmcPtr = removeNonLocalBCs(&cells, pmcPtr);
	   nPMC = pmcPtr.size();
	   PMCe = new uint[nPMC];
	   PMCf = new uint[nPMC];
	   // Stores solver rel pos and faces.
	   for (uint i = 0; i < pmcPtr.size(); i++) {
	      PMCe[i] = cells.getRelPosOfId(pmcPtr[i]->getCell()->getId());
	      PMCf[i] = pmcPtr[i]->getFace();
	   }
	}
	{
	   for (uint i = 0; i < smb_->pMGroup->countSIBC(); i++) {
	      const PMSurfaceSIBC* m =
	        dynamic_cast<const PMSurfaceSIBC*>(smb_->pMGroup->getPMSurface(i));
	      if (m != NULL) {
	         vector<const BoundaryCondition*> sibcPtr
	          = bc.getMatId(m->getId());
	         sibcPtr = removeNonLocalBCs(&cells, sibcPtr);
	         dispersive.push_back(
	          new DGSIBC(*m, sibcPtr, cells, map, vmapM,
	                ExP, EyP, EzP, HxP, HyP, HzP));
	      }
	   }
	}
}

void
DGExplicit::buildEMSources(
 const EMSourceGroup& em,
 const BCGroup& bc,
 const MapGroup& maps,
 const CellGroup& cells) {
	// Copies the sources structure into solver.
	if (em.countPlaneWaves() > 0) {
		vector<const BoundaryCondition*> aux =	 bc.get(Condition::emSource);
		source.push_back(new DGPlaneWave(*em.getPlaneWave(), aux, maps,
		 cells, comm, dE, dH, vmapM));
	}
	for (uint i = 0; i < em.countDipoles(); i++) {
	   vector<const BoundaryCondition*> aux = bc.get(Condition::emSource);
	   source.push_back(
	    new DGDipole(*em.getDipole(i), aux, maps, cells, dE, dH, vmapM));
	}
	for (uint i = 0; i < em.countWaveports(); i++) {
		vector<const BoundaryCondition*> aux =	 bc.get(Condition::emSource);
		Waveport::Shape shape = em.getWaveport(i)->getShape();
		if (shape == Waveport::rectangular) {
			source.push_back(new DGWaveportRectangular(
			 *em.getWaveport(i), aux, maps, cells, dE, dH, vmapM));
		} else {
			cerr << endl << "ERROR @ buildEMSources" << endl;
			cerr << endl << "Unreckognized waveport shape." << endl;
		}
	}
}



void
DGExplicit::buildScalingFactors(
 const CellGroup& cells,
 const MapGroup& map) {
	buildFieldScalingFactors(cells);
	buildFluxScalingFactors(cells, map);
	buildCurvedFluxScalingFactors(cells, map);
}

void
DGExplicit::buildCurvedFluxScalingFactors(
 const CellGroup& cells,
 const MapGroup& map) {
	// Counts curved faces.
	nCurvedFaces = 0;
	for (uint e = 0; e < nK; e++) {
		uint id = cells.getIdOfRelPos(e);
		const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
		for (uint f = 0; f < cell->getFaces(); f++) {
			if (cell->isCurvedFace(f)) {
				nCurvedFaces++;
			}
		}
	}
	curveFace = new DGCurvedFace[nCurvedFaces];
	// Supress linear fluxes operators. Computes curved operators.
	uint face = 0;
	for (uint e = 0; e < nK; e++) {
		uint id = cells.getIdOfRelPos(e);
		const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
		double impM, admM, impP, admP, impAv, admAv;
		for (uint f = 0; f < faces; f++) {
			if (cell->isCurvedFace(f)) {
				// Builds CurvedFace information
				impM = cell->material->impedance;
				admM = cell->material->admitance;
				uint nId = map.getNeighbour(cell->getId(), f)->getId();
				const CellTet<ORDER_N>* neigh = cells.getPtrToCellWithId(nId);
				impP = neigh->material->impedance;
				admP = neigh->material->admitance;
				impAv = (impM + impP) * 0.5;
				admAv = (admM + admP) * 0.5;
				curveFace[face++] = DGCurvedFace(
				 cell, f, e, rhsE, rhsH, dE, dH, dresE, dresH,
				 impP, admP, impAv, admAv);
				uint i = e * faces + f;
				// Sets flux scaling factors to zero.
				CVecR3 zero(0.0, 0.0, 0.0);
				nAdm.set(i, zero);
				nImp.set(i, zero);
				cnAdm.set(i, zero);
				cnImp.set(i, zero);
				rnAdm.set(i, zero);
				rnImp.set(i, zero);
			}
		}
	}
}

void
DGExplicit::buildMaterials(
 const CellGroup& cells,
 const ArgumentsCudg3d* arg) {
   const PhysicalModelGroup* pm = smb_->pMGroup;
	const uint nMat = pm->count();
	// Creates Dispersive materials vars parameters and stores ptrs.
	for (uint i = 0; i < nMat; i++) {
		const PhysicalModel* m = pm->get(i);
		if (m->isElectricalDispersive()) {
			dispersive.push_back(
			 new DGDispersiveVolumic(*pm->getPMVolumeDispersiveWithId(m->getId()), cells));
		}
	}
	// Creates PML materials variables parameters and stores pointers.
	for (uint i = 0; i < pm->countAssignedPML(); i++) {
		const bool isConstCond = arg->isPmlUseConstantConductivity();
		const double cond = arg->getPmlConductivity();
		const PMVolumePML* m = pm->getAssignedPML(i);
		switch (m->getOrientation()) {
		case PMVolumePML::PMLx:
			dispersive.push_back(new DGPMLx(*m,cells,isConstCond,cond));
			break;
		case PMVolumePML::PMLy:
			dispersive.push_back(new DGPMLy(*m,cells,isConstCond,cond));
			break;
		case PMVolumePML::PMLz:
			dispersive.push_back(new DGPMLz(*m,cells,isConstCond,cond));
			break;
		case PMVolumePML::PMLxy:
			dispersive.push_back(new DGPMLxy(*m,cells,isConstCond,cond));
			break;
		case PMVolumePML::PMLyz:
			dispersive.push_back(new DGPMLyz(*m,cells,isConstCond,cond));
			break;
		case PMVolumePML::PMLzx:
			dispersive.push_back(new DGPMLzx(*m,cells,isConstCond,cond));
			break;
		case PMVolumePML::PMLxyz:
			dispersive.push_back(new DGPMLxyz(*m,cells,isConstCond,cond));
			break;
		default:
			break;
		}
	}
}

bool
DGExplicit::checkPtrsToNeigh() const {
	bool res = true;
	for (uint e = 0; e < nK; e++) {
		for (uint f = 0; f < faces; f++) {
			bool problem = false;
			problem |= (ExP[e][f] == NULL);
			problem |= (EyP[e][f] == NULL);
			problem |= (EzP[e][f] == NULL);
			problem |= (HxP[e][f] == NULL);
			problem |= (HyP[e][f] == NULL);
			problem |= (HzP[e][f] == NULL);
			if (problem) {
				res = false;
				cout<< "ERROR@checkPtrsToNeigh" << endl;
				cout<< "Ptr to neigh " << e << "face " << f <<
				 " not assigned in solver init." << endl;
			}
		}
	}
	return res;
}

void
DGExplicit::deduplicateVMaps(const CellGroup& cells) {
	// --- Copies vmapM -----------------------------------------------
	SimplexTet<ORDER_N> tet;
	for (uint f = 0; f < faces; f++) {
		for (uint i = 0; i < nfp; i++) {
			vmapM[f][i] = tet.sideNode(f,i);
		}
	}
	// --- Copies vmapP -----------------------------------------------
	// Allocates space for the deduplicated vmapP and inits all to -1.
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < int(nfp); j++) {
			vmapP[i][j] = -1;
		}
	}
	// deduplicates vmapP in a list and points cell->vmap to them.
	for (uint e = 0; e < nK; e++) {
		uint id = cells.getIdOfRelPos(e);
		const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
		for (uint f = 0; f < faces; f++) {
			// Checks if the vmapP[f] vector is in the list.
			bool found;
			for (int i = 0; i < 16; i++) {
				found = true;
				for (uint j = 0; j < nfp; j++)
					if (vmapP[i][j] != int(cell->vmapP[f][j])) {
						found = false;
						break;
					}
				// Make the elements to point to their corresponding vmapP.
				if (found) {
					map[e][f] = vmapP[i];
					break;
				}
			}
			// If not in the list, is added to the first empty row.
			if (!found) {
				for (int i = 0; i < 16; i++) {
					if (vmapP[i][0] == -1) {
						for (uint j = 0; j < nfp; j++) {
							vmapP[i][j] = cell->vmapP[f][j];
						}
						// Points cell->vmapP to its corresponding one.
						map[e][f] = vmapP[i];
						break;
					}
				}
			}
		}
	}
	// Checks that all the elem vmapP pointers have been correctly addressed.
	bool problem = false;
	for (uint e = 0; e < nK; e++) {
		for (uint f = 0; f < faces; f++) {
			if (map[e][f] == NULL) {
				problem = true;
				if (problem) {
					cerr << endl << "ERROR: Solver::deduplicateVMaps()"
					 << "An vmapP has not been set. Elem "
					 << e << ", face " << f << endl;
				}
			}
		}
	}
	// Checks that vmapP has the correct values.
	for (uint e = 0; e < nK; e++) {
		uint id = cells.getIdOfRelPos(e);
		const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
		for (uint f = 0; f < faces; f++) {
			for (uint i = 0; i < nfp; i++) {
				if (int(cell->vmapP[f][i]) != map[e][f][i]) {
					if (!problem) {
						cerr << endl << "ERROR: Solver::deduplicateVMaps" << endl;
						cerr << endl << "Check vmapsP" << endl;
					}
					cerr << endl << "Elem " << e << ", face " << f << endl;
					problem = true;
				}
			}
		}
	}
}

vector<const BoundaryCondition*>
DGExplicit::removeNonLocalBCs(
 const CellGroup* cells,
 const vector<const BoundaryCondition*>& bc) const {
	vector<const BoundaryCondition*> res;
	res.reserve(bc.size());
	for (uint i = 0; i < bc.size(); i++) {
		uint id = bc[i]->getCell()->getId();
		if (cells->isLocalId(id)) {
			res.push_back(bc[i]);
		}
	}
	return res;
}

void
DGExplicit::LTSSaveFieldsAndResidues(
 const uint fKSave,
 const uint lKSave) {
	const uint init = fKSave * np;
	const uint end = lKSave * np;
	savedE.copy(init, end, E);
	savedH.copy(init, end, H);
	savedResE.copy(init, end, resE);
	savedResH.copy(init, end, resH);
}

void
DGExplicit::LTSLoadFieldsAndResidues(
 const uint fKLoad,
 const uint lKLoad) {
	const uint init = fKLoad * np;
	const uint end = lKLoad * np;
	E.copy(init, end, savedE);
	H.copy(init, end, savedH);
	resE.copy(init, end, savedResE);
	resH.copy(init, end, savedResH);
}


void DGExplicit::allocateFieldsForLTS() {
	dresE.setSize(nK*nfp*4);
	dresH.setSize(nK*nfp*4);
	savedE.setSize(getFieldDOFs()/3);
	savedH.setSize(getFieldDOFs()/3);
	savedResE.setSize(getFieldDOFs()/3);
	savedResH.setSize(getFieldDOFs()/3);
}

void
DGExplicit::copyJumpsToResidueJumps(
 const uint e1,
 const uint e2) {
	uint i, j, f, e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(e,i,f,j)
	#endif
	for (e = e1; e < e2; e++) {
		i = e * nfp * faces;
		for (f = 0; f < faces; f++) {
			for (j = 0; j < nfp; j++) {
				dresE.set(x)[i] = dE(x)[i];
				dresE.set(y)[i] = dE(y)[i];
				dresE.set(z)[i] = dE(z)[i];
				dresH.set(x)[i] = dH(x)[i];
				dresH.set(y)[i] = dH(y)[i];
				dresH.set(z)[i] = dH(z)[i];
				i++;
			}
		}
	}
}


void
DGExplicit::addRHSToResidueElectric(
 const uint e1, const uint e2,	const double rkdt) {
	uint i, j, e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(i,j,e)
	#endif
	for (e = e1; e < e2; e++) {
		i = e * np;
		for (j = 0; j < np; j++) {
			resE.set(x)[i] += rhsE(x)[i] * rkdt;
			resE.set(y)[i] += rhsE(y)[i] * rkdt;
			resE.set(z)[i] += rhsE(z)[i] * rkdt;
			i++;
		}
	}
}

void
DGExplicit::addRHSToResidueMagnetic(
 const uint e1, const uint e2, const double rkdt) {
	uint i, j, e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(i,j,e)
	#endif
	for (e = e1; e < e2; e++) {
		i = e * np;
		for (j = 0; j < np; j++) {
			resH.set(x)[i] += rhsH(x)[i] * rkdt;
			resH.set(y)[i] += rhsH(y)[i] * rkdt;
			resH.set(z)[i] += rhsH(z)[i] * rkdt ;
			i++;
		}
	}
}

void
DGExplicit::updateFieldsWithRes(
 const uint e1,
 const uint e2,
 const double rkb) {
	updateFieldsWithResBase(e1,e2,rkb);
#ifndef SOLVER_IGNORE_DISPERSIVES
	for (uint d = 0; d < dispersive.size(); d++) {
		dispersive[d]->updateWithRes(e1,e2,rkb);
	}
#endif
}

void
DGExplicit::addRHSToRes(
 const uint e1,
 const uint e2,
 const double rka,
 const double dt) {
	uint i = getIndexOfElement(e1);
	uint j = getIndexOfElement(e2);
#	ifdef SOLVER_USE_OPENMP
	resE.prod_omp(i,j, rka);
	resE.addProd_omp(i,j, rhsE, dt);
	resH.prod_omp(i,j, rka);
	resH.addProd_omp(i,j, rhsH, dt);
#	else
	resE.prod(i,j, rka);
	resE.addProd(i,j, rhsE, dt);
	resH.prod(i,j, rka);
	resH.addProd(i,j, rhsH, dt);
#	endif
	// Polarization currents in dispersive materials.
#	ifndef SOLVER_IGNORE_DISPERSIVES
	for (uint d = 0; d < dispersive.size(); d++) {
		dispersive[d]->addRHSToRes(e1,e2,rka,dt);
	}
#	endif
}
