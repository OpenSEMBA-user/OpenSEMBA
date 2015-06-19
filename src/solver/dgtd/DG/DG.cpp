
#ifndef SOLVER_H_
#include "DG.h"
#endif
// =============== Solver methods =============================================
DG::DG() {
   nK = 0;
   buildLIFT();
}

DG::~DG() {
}

void
DG::setFieldsToZero() {
   E.setToZero();
   H.setToZero();
}

void
DG::setFieldsToOne() {
   E.setToOne();
   H.setToZero();
}

void
DG::setFieldsToRandom() {
   static const double min = -1.0;
   static const double max = 1.0;
   E.setToRandom(min, max);
   H.setToRandom(min, max);
}

void
DG::setFieldsToGaussian(
      const CellGroup& cells,
      const double amplitude,
      CVecR3& polarization,
      const CVecR3& gaussCenter,
      const double gaussWidth) {
   CartesianVector <double,3> aux;
   double expArg;
   polarization.normalize();
   for (uint e = 0; e < nK; e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      for (uint i = 0; i < np; i++) {
         aux = cell->n[i] - gaussCenter;
         expArg = aux.norm() / gaussWidth;
         E.set(e*np + i, polarization*amplitude*exp(- expArg * expArg));
      }
   }
   H.setToZero();
}

void
DG::setFieldsToHarmonics(
      const CellGroup& cells,
      const CartesianVector<int,3>& harmonics,
      CVecR3& polarization) {
   double amp;
   CVecR3 pos;
   polarization.normalize();
   for (uint e = 0; e < nK; e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      for (uint i = 0; i < np; i++) {
         pos = cell->n[i];
         if(harmonics(1) == 0) {
            amp = sin(pos(0) * harmonics(0) * M_PI);
         } else {
            amp = sin(pos(0)*harmonics(0)*M_PI)
				             * sin(pos(1)*harmonics(1)*M_PI);
         }
         E.set(e*np + i, polarization*amp);
      }
   }
   H.setToZero();
}

void
DG::setFieldsAndTimeFromResumeFile() {
   string resumeFileName = "simulation.resume";
   ifstream f_in(resumeFileName.c_str());
   // Checks file existence.
   if (!f_in) {
      cerr << endl << "ERROR @ Solver::setFieldsFromResumeFile: "
            << "File " << resumeFileName
            << " does not exist, imposible to resume." << endl;
   }
   // Reads result header.
   string trash;
   getline(f_in, trash);
   {
      // Reads electric resume fields.
      VectorModuleResult electricField(nK * np);
      electricField.readResult(f_in);
      // Copies result electric fields into the fast solver field vectors.
      for (uint i = 0; i < nK * np; i++) {
         E.set(x)[i] = electricField.values[0][i];
         E.set(y)[i] = electricField.values[1][i];
         E.set(z)[i] = electricField.values[2][i];
      }
   }
   {
      // Reads magnetic resume fields.
      VectorModuleResult magneticField(nK * np);
      magneticField.readResult(f_in);
      // Copies result electric fields into the fast solver field vectors.
      for (uint i = 0; i < nK * np; i++) {
         H.set(x)[i] = magneticField.values[0][i];
         H.set(y)[i] = magneticField.values[1][i];
         H.set(z)[i] = magneticField.values[2][i];
      }
   }
   f_in.close();
}
void
DG::buildFieldScalingFactors(
      const CellGroup& cells) {
   oneOverEps = new double[nK];
   oneOverMu = new double[nK];
   for (uint e = 0; e < nK; e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      oneOverEps[e] = 1.0 /
            (cell->material->getRelativePermittivity()*Constants::eps0);
      oneOverMu[e]  = 1.0 /
            (cell->material->getRelativePermeability()*Constants::mu0);
   }
}

void
DG::buildFluxScalingFactors(
      const CellGroup& cells,
      const MapGroup& map) {
   nAdm.setSize(nK*4);
   nImp.setSize(nK*4);
   rnAdm.setSize(nK*4);
   rnImp.setSize(nK*4);
   cnAdm.setSize(nK*4);
   cnImp.setSize(nK*4);
   // Straight faces -------------------------------------------------
   for (uint e = 0; e < nK; e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      double impM, admM, impP, admP, impAv, admAv;
      CVecR3 n, rn, cn;
      CVecR3 nAdmAux, rnAdmAux, cnAdmAux;
      CVecR3 nImpAux, rnImpAux, cnImpAux;
      for (uint f = 0; f < faces; f++) {
         uint i = e * faces + f;
         // Computes Scaling factor.
         double fSc = 0.5 * cell->getAreaOfFace(f) / cell->getVolume();
         // Computes local impedance and admittance..
         impM = cell->material->impedance;
         admM = cell->material->admitance;
         // Computes contiguous element impedance and admittance.
         uint neighId = map.getNeighbour(cell->getId(), f)->getId();
         const CellTet<ORDER_N>* neigh =	 cells.getPtrToCellWithId(neighId);
         impP = neigh->material->impedance;
         admP = neigh->material->admitance;
         impAv = (impM + impP) * 0.5;
         admAv = (admM + admP) * 0.5;
         // ----- Computes vectors ---------------------------------
         n = cell->getSideNormal(f);
         // Computes: rn = 1 - n .^ 2.
         rn = double(1.0);
         rn -= n * n; // This is NOT cartesian scalar prod.
         // Computes: cn = [ ny*nz  nz*nx nx*ny ].
         cn(0) = n(0) * n(1);
         cn(1) = n(1) * n(2);
         cn(2) = n(2) * n(0);
         // --- Assigns flux scaling factor ---
         nAdm.set(i, n  * fSc * (admP / admAv));
         nImp.set(i, n  * fSc * (impP / impAv));
         cnAdm.set(i, cn * fSc / admAv);
         cnImp.set(i, cn * fSc / impAv);
         rnAdm.set(i, rn * fSc / admAv);
         rnImp.set(i, rn * fSc / impAv);
      }
   }
}

void
DG::init(
      const ArgumentsCudg3d* arg,
      const PMGroup* pm_,
      const CellGroup* cells, Comm* comm_) {
   comm = comm_;
   upwinding = arg->getUpwinding();
   nK = cells->getLocalSize();
   buildMaterials(*cells, arg);
   buildCMatrices(*cells);
   allocateFieldsAndRes();
   setResidualsToZero();
}

void
DG::addFluxesToRHS(
      const uint e1,
      const uint e2,
      const double localTime,
      const double minDT) {
   computeJumps(e1,e2, localTime,minDT);
   addFluxesToRHSElectric(e1,e2);
   addFluxesToRHSMagnetic(e1,e2);
}

void
DG::buildCMatrices(const CellGroup& cells) {
   uint e;
#	ifdef SOLVER_DEDUPLICATE_OPERATORS
   for (e = 0; e < nK; e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      StaMatrix<double,np,np> C[3];
      cell->getCMatrices(C);
      for (uint i = 0; i < 3; i++) {
         CList.insert(C[i]);
      }
   }
#	else
   CList = new StaMatrix<double,np,np>[3*nK];
#	ifdef SOLVER_USE_OPENMP
#	pragma omp parallel for private(e)
#	endif
   for (e = 0; e < nK; e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      StaMatrix<double,np,np> C[3];
      cell->getCMatrices(C);
      for (uint i = 0; i < 3; i++) {
         uint j = 3 * e + i;
         CList[j] = C[i];
      }
   }
#	endif
   assignMatrices(cells);
}

void
DG::computeCurlsInRHS(
      const uint e1,
      const uint e2) {
   computeCurlsInRHSElectric(e1, e2);
   computeCurlsInRHSMagnetic(e1, e2);
}

void
DG::updateFieldsWithResBase(
      const uint e1,
      const uint e2,
      const double rkb) {
   uint i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
   for (e = e1; e < e2; e++) {
      i = e * np;
      for (j = 0; j < np; j++) {
         E.set(x)[i] += resE(x)[i] * rkb;
         E.set(y)[i] += resE(y)[i] * rkb;
         E.set(z)[i] += resE(z)[i] * rkb;
         H.set(x)[i] += resH(x)[i] * rkb;
         H.set(y)[i] += resH(y)[i] * rkb;
         H.set(z)[i] += resH(z)[i] * rkb;
         i++;
      }
   }
}


void
DG::copyFieldsInResidues(
      const uint e1,
      const uint e2) {
   const uint init = e1 * np;
   const uint end = e2 * np;
   resE.copy(init, end, E);
   resH.copy(init, end, H);
}

void
DG::swapResiduesAndFields(
      const uint e1,
      const uint e2) {
   const uint init = e1 * np;
   const uint end = e2 * np;
   E.swap(resE, init, end);
   H.swap(resH, init, end);
}

void
DG::buildScalingFactors(
      const CellGroup& cells,
      const MapGroup& map) {
   buildFieldScalingFactors(cells);
   buildFluxScalingFactors(cells, map);
}

void
DG::buildLIFT() {
   // Copies LIFT matrices into the solver matrix format.
   // These are used only for linear elements.
   static const SimplexTet<ORDER_N> tet;
   StaMatrix<double, np, nfp * 4> tmpLIFT;
   for (uint i = 0; i < np; i++) {
      for (uint f = 0; f < faces; f++) {
         for (uint j = 0; j < nfp; j++) {
            tmpLIFT(i, f * nfp + j) = tet.LIFT[f](i, j);
         }
      }
   }
   tmpLIFT.convertToArray(MATRICES_ROW_MAJOR, LIFT);
}

void
DG::allocateFieldsAndRes() {
   uint dof = getFieldDOFs();
   E.setSize(dof/3);
   H.setSize(dof/3);
   resE.setSize(dof/3);
   resH.setSize(dof/3);
}

void
DG::setResidualsToZero() {
   resE.setToZero();
   resH.setToZero();
}

const FieldR3* DG::getElectric() const {
   return &E;
}

const FieldR3* DG::getMagnetic() const {
   return &H;
}

uint
DG::getGlobalFieldPosOfVertex(pair<const ElemR*, uint> vertex) const {
   uint e = getGlobalRelPosOfId(vertex.first->getId());
   static const SimplexTet<ORDER_N> tet;
   return (e*tet.np + tet.vertex(vertex.second));
}

vector<uint> DG::getGlobalFieldPosOfFace(Face bound) const {
   const uint e = getGlobalRelPosOfId(bound.first->getId());
   const uint f = bound.second;
   static const SimplexTet<ORDER_N> tet;
   vector<uint> res(tet.nfp);
   for (uint i = 0; i < tet.nfp; i++) {
      res[i] = e * tet.np + tet.sideNode(f,i);
   }
   return res;
}

vector<uint> DG::getGlobalFieldPosOfVolume(const uint volId) const {
   const uint e = getGlobalRelPosOfId(volId);
   static const SimplexTet<ORDER_N> tet;
   vector<uint> res(tet.np);
   for (uint i = 0; i < tet.np; i++) {
      res[i] = e * tet.np + i;
   }
   return res;
}
