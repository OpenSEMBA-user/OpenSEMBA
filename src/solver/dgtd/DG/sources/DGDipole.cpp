#include "DGDipole.h"

DGDipole::DGDipole(
      const Dipole& dip,
      const vector<const BoundaryCondition*>& bc,
      const MapGroup& map,
      const CellGroup& cells,
      FieldD3& dE, FieldD3& dH,
      const int vmapM[faces][nfp])
: Dipole(dip) {
   initSource(bc, map, cells, dE, dH, vmapM);
   // Determines total or scattered fields in the bc.
   if (nETFNB != 0) {
      cerr << endl << "ERROR @ SolveDipole::build(): "
       << "Trying to set TF/SF in a not backed boundary." << endl;
   }
   // Total field boundary.
   vector<pair<uint, uint> > total;
   total = getElemFaces(bc, map, cells, totalField);
   tPos = new SphericalVector[nETF * nfp];
   for (uint i = 0; i < total.size(); i++) {
      uint id = cells.getIdOfRelPos(total[i].first);
      uint f = total[i].second;
      for (uint j = 0; j < nfp; j++) {
         tPos[i*nfp+j] =
               cells.getPtrToCellWithId(id)->getSideNodePos(f,j) - position_;
      }
   }
   // Scattered field boundary.
   vector<pair<uint,uint> > scatt;
   scatt = getElemFaces(bc, map, cells, scatteredField);
   sPos = new SphericalVector[nESF * nfp];
   for (uint i = 0; i < scatt.size(); i++) {
      uint id = cells.getIdOfRelPos(scatt[i].first);
      uint f = scatt[i].second;
      for (uint j = 0; j < nfp; j++) {
         sPos[i*nfp+j] =
               cells.getPtrToCellWithId(id)->getSideNodePos(f,j) - position_;
      }
   }
}

DGDipole::~DGDipole() {
}

void
DGDipole::computeExcitation(
      const double time,
      const double minDT) {
   computeExcitationField(
         ExTInc, EyTInc, EzTInc, HxTInc, HyTInc, HzTInc,
         tPos, nETF, time);
   computeExcitationField(
         ExSInc, EySInc, EzSInc, HxSInc, HySInc, HzSInc,
         sPos, nESF, time);
}

void
DGDipole::computeExcitationField(
      double* ExInc, double* EyInc, double* EzInc,
      double* HxInc, double* HyInc, double* HzInc,
      const SphericalVector* vPos,
      const uint nE,
      const double time) const {
   // PURPOSE: Computes the dipole excitation.
   // Chapter 2, R. Gomez's book. 2006.
   // "Electromagnetic Field Theory for physicist and engineers.
   // Section: Fields created by an infinitesimal current element with
   // arbitrary time dependence.
   // I use the derivative of the gaussian.
   // Otherwise charge accumulates and fields do not tend to zero.
   // This also saves some memory because we do not have to save data
   // from time integrals of the charge.
   double pos, pos2, pos3;
   double expArg, expArg2;
   double iT, iD;
   double tDelayed, sint, cost;
   SphericalVector sphE, sphH;
   CVecD3 E, H;
   // External field.
   const uint nFields = nfp * nE;
   for (uint j = 0; j < nFields; j++) {
      pos = vPos[j].norm();
      pos2 = pos * pos;
      pos3 = pos2 * pos;
      sint = sin(vPos[j].theta);
      cost = cos(vPos[j].theta);
      tDelayed = time - pos / SPEED_OF_LIGHT; // Delayed time.
      expArg = (tDelayed - gaussDelay_) / (spreadSqrt2_);
      expArg2 = expArg * expArg;
      iT = exp(- expArg2); // current @ delayed time.
      iD = - iT * 2.0 * expArg / spreadSqrt2_; // derivative of current.
      double iD2 = iD * (-2.0)* expArg / spreadSqrt2_
            + iT * (-2.0) / spreadSqrt2_ / spreadSqrt2_;
      double er = length_ * INV4PIEPS0 * 2.0 * cost
            * (iT/pos3 + iD/(SPEED_OF_LIGHT*pos2));
      double et = length_ * INV4PIEPS0 * sint
            * (iT/pos3
                  + iD/(SPEED_OF_LIGHT * pos2)
                  + iD2/(SPEED_OF_LIGHT_SQ*pos) );
      double hp = length_ * INV4PI * sint
            * (iD2/(pos*SPEED_OF_LIGHT) + iD/pos2 );
      // Spherical to Cartesian conversion.
      E = vPos[j].convertSphericalVectorField(er, et, 0.0);
      H = vPos[j].convertSphericalVectorField(0.0, 0.0, hp);
      // TODO: Orientation of dipole can be included here
      // use a rotation matrix applied on E and H.
      ExInc[j] = E(0);
      EyInc[j] = E(1);
      EzInc[j] = E(2);
      HxInc[j] = H(0);
      HyInc[j] = H(1);
      HzInc[j] = H(2);
   }
}

void
DGDipole::printInfo() const {
   cout << " --- SolverDipole Info ---" << endl;
   Dipole::printInfo();
   cout << "#ETF: " << nETF << endl;
   cout << "#ESF: " << nESF << endl;
}
