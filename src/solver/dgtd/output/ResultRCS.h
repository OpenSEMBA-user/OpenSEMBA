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
//   int num;
//   double time;
//   CVecR3 incident;
//   vector<CVecR3 > electric, magnetic;
//   RCSStep_s(
//         int num_,
//         double time_,
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
//   uint nSteps;
//   vector<RCSStep_s*> step;
//   // Returns a vector of size Nsteps.
//
//   vector<double>
//   getTimeVector() const {
//      vector<double> res(nSteps);
//      for (uint i = 0; i < nSteps; i++) {
//         res[i] = step[i]->time;
//      }
//      return res;
//   }
//
//   vector<CVecR3 >
//   getIncidentField() const {
//      vector<CVecR3 > res(nSteps);
//      for (uint i = 0; i <nSteps; i++) {
//         res[i] = step[i]->incident;
//      }
//      return res;
//   }
//
//   vector<CVecR3 >
//   getElectricField(const uint node) const {
//      vector<CVecR3 > res(nSteps);
//      for (uint i = 0; i < nSteps; i++) {
//         res[i] = step[i]->electric[node];
//      }
//      return res;
//   }
//
//   vector<CVecR3 >
//   getMagneticField(const uint node) const {
//      vector<CVecR3 > res(nSteps);
//      for (uint i = 0; i < nSteps; i++) {
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
//         const MeshVolume& mesh,
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
//         const double time);
//   vector<CartesianVector<complex<double>,3> >
//   nearToFarField(
//         const double frequency,
//         const vector<pair<double,double> >& directions) const;
//   vector<double>
//   getRCS(
//         const double frequency,
//         const vector<pair<double, double> >& directions) const;
//   double
//   getRCS(
//         const double frequency,
//         const pair<double, double>& directions) const;
//   void
//   printInfo() const;
//private:
//   static const uint N = ORDER_N;
//   static const uint np = (N+1) * (N+2) * (N+3) / 6;
//   static const uint nfp = (N+1) * (N+2) / 2;
//   static const uint estimatedNumberOfSteps = 5000;
//   uint numNodes;
//   uint nElem;
//   vector<pair<uint, uint> > elemFace;
//   vector<CoordD3 > vertex;
//   bool quadraticMesh;
//   RCSStepGroup_s stepsGroup;
//   void
//   writeHeader(const vector<pair<const Volume*, uint> >& cellsBorder);
//   void
//   readHeader();
//   void
//   readFields();
//   double
//   getSamplingTime() const;
//   void
//   applySymmetries(const Symmetry::Symmetry sym[3]);
//};
#endif /* RCSRESULT_H_ */
