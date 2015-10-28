// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * RCSResult.h
 *
 *  Created on: Nov 2, 2012
 *      Author: luis
 */

#ifndef RESULTRCS_H_
#define RESULTRCS_H_

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <assert.h>

//#include "../../../common/exporter/Result.h"
//#include "../../../common/math/MathUtils.h"
//#include "../../../common/inputs/electromagneticSources/EMSource.h"
//#include "../core/BoundaryCondition.h"
//#include "../core/CellTri3.h"
//#include "../core/CellTri6.h"
//#include "../DG/DG.h"
//
//using namespace std;
//
//namespace Symmetry {
//typedef enum {
//   undefined,
//   none,
//   pec,
//   pmc
//} Symmetry;
//}
//
//#ifndef LABEL_ENDING
//#define LABEL_ENDING ((char) ':')
//#endif
//
//struct RCSStep_s {
//   Int num;
//   Real time;
//   CVecR3 incident;
//   vector<CVecR3 > electric, magnetic;
//   RCSStep_s(
//         Int num_,
//         Real time_,
//         CVecR3& incident_,
//         vector<CVecR3 >& electric_,
//         vector<CVecR3 >& magnetic_) {
//      num = num_;
//      time = time_;
//      incident = incident_;
//      electric = electric_;
//      magnetic = magnetic_;
//   }
//};
//
//struct RCSStepGroup_s {
//   UInt nSteps;
//   vector<RCSStep_s*> step;
//   // Returns a vector of size Nsteps.
//
//   vector<Real>
//   getTimeVector() const {
//      vector<Real> res(nSteps);
//      for (UInt i = 0; i < nSteps; i++) {
//         res[i] = step[i]->time;
//      }
//      return res;
//   }
//
//   vector<CVecR3 >
//   getIncidentField() const {
//      vector<CVecR3 > res(nSteps);
//      for (UInt i = 0; i <nSteps; i++) {
//         res[i] = step[i]->incident;
//      }
//      return res;
//   }
//
//   vector<CVecR3 >
//   getElectricField(const UInt node) const {
//      vector<CVecR3 > res(nSteps);
//      for (UInt i = 0; i < nSteps; i++) {
//         res[i] = step[i]->electric[node];
//      }
//      return res;
//   }
//
//   vector<CVecR3 >
//   getMagneticField(const UInt node) const {
//      vector<CVecR3 > res(nSteps);
//      for (UInt i = 0; i < nSteps; i++) {
//         res[i] = step[i]->magnetic[node];
//      }
//      return res;
//   }
//};
//
//class ResultRCS : public ProjectFile, public Result {
//public:
//   ResultRCS(
//         const string& fileName,
//         const string& resultName,
//         const MeshUnstructured& mesh,
//         const vector<const BoundaryCondition*>& iB);
//   ResultRCS(
//         const string& filename,
//         const Symmetry::Symmetry sym[3]);
//   virtual ~ResultRCS();
//   void
//   writeResult(
//         const FieldR3& elec,
//         const FieldR3& magn,
//         const CVecR3& EInc,
//         const Real time);
//   vector<CVecC3 >
//   nearToFarField(
//         const Real frequency,
//         const vector<pair<Real,Real> >& directions) const;
//   vector<Real>
//   getRCS(
//         const Real frequency,
//         const vector<pair<Real, Real> >& directions) const;
//   Real
//   getRCS(
//         const Real frequency,
//         const pair<Real, Real>& directions) const;
//   void
//   printInfo() const;
//private:
//   static const UInt N = ORDER_N;
//   static const UInt np = (N+1) * (N+2) * (N+3) / 6;
//   static const UInt nfp = (N+1) * (N+2) / 2;
//   static const UInt estimatedNumberOfSteps = 5000;
//   UInt numNodes;
//   UInt nElem;
//   vector<pair<UInt, UInt> > elemFace;
//   vector<CoordR3 > vertex;
//   bool quadraticMesh;
//   RCSStepGroup_s stepsGroup;
//   void
//   writeHeader(const vector<pair<const Volume*, UInt> >& cellsBorder);
//   void
//   readHeader();
//   void
//   readFields();
//   Real
//   getSamplingTime() const;
//   void
//   applySymmetries(const Symmetry::Symmetry sym[3]);
//};
#endif /* RCSRESULT_H_ */
