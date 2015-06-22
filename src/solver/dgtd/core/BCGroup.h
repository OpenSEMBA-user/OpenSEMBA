/*
 * BCGroup.h
 *
 *  Created on: Jul 8, 2013
 *      Author: luis
 */

#ifndef BCGROUP_H_
#define BCGROUP_H_

#include "BoundaryCondition.h"
#include "../../../common/SmbData.h"

//using namespace std;
//
//struct lexCompareBC {
//   bool
//   operator() (
//         const BoundaryCondition* lhs,
//         const BoundaryCondition* rhs) const {
//      if (lhs->getCell()->getId() < rhs->getCell()->getId()) {
//         return true;
//      }
//      if (lhs->getCell()->getId() == rhs->getCell()->getId()) {
//         if (lhs->getFace() < rhs->getFace()) {
//            return true;
//         }
//      }
//      return false;
//   }
//};
//
//class BCGroup {
//public:
//   BCGroup(const SmbData* smb);
//   vector<const BoundaryCondition*>
//   get(const Condition::Type type) const;
//   vector<const BoundaryCondition*>
//   getMatId(const UInt id) const;
//   void
//   printInfo() const;
//private:
//   set<BoundaryCondition*, lexCompareBC> bc_;
//   void
//   buildEMSourceBC(
//         const MeshVolume& mesh,
//         const EMSourceGroup& em);
//   void
//   buildPhysicalModelBC(
//         const MeshVolume& mesh,
//         const PMGroup& pm);
//   void
//   addBC(const MeshVolume& mesh,
//         const PMGroup& em,
//         const vector<const SurfR*>& surfs);
//};
//
#endif /* BCGROUP_H_ */
