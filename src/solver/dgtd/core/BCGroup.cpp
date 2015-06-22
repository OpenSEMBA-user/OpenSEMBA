///*
// * BCGroup.cpp
// *
// *  Created on: Jul 8, 2013
// *      Author: luis
// */
//
//#include "BCGroup.h"
//
//BCGroup::BCGroup(
//      const SmbData* smb) {
//   buildPhysicalModelBC(*smb->mesh, *smb->pMGroup);
//   buildEMSourceBC(*smb->mesh, *smb->emSources);
//}
//
//void BCGroup::addBC(
//   const MeshVolume& mesh,
//   const PMGroup& pm,
//   const vector<const SurfR*>& surf) {
//   for (UInt i = 0; i < surf.size(); i++) {
//      const UInt id = surf[i]->getId();
//      const Condition* condition = pm.getPMWithId(surf[i]->getMatId());
//      pair<const Volume*, UInt> boundary = mesh.map.getInnerFace(id);
//      bc_.insert(new BoundaryCondition(boundary, condition));
//      if (!mesh.map.isDomainBoundary(boundary)) {
//         pair<const Volume*, UInt> boundary = mesh.map.getOuterFace(id);
//         bc_.insert(new BoundaryCondition(boundary, condition, true));
//      }
//   }
//}
//
//void
//BCGroup::buildPhysicalModelBC(
//      const MeshVolume& mesh,
//      const PMGroup& pm) {
//   {
//      vector<UInt> ids = pm.getIds(Condition::pec);
//      addBC(mesh, pm, mesh.elem_.getSurfacesWithMatId(ids));
//   }
//   {
//      vector<UInt> ids = pm.getIds(Condition::pmc);
//      addBC(mesh, pm, mesh.elem_.getSurfacesWithMatId(ids));
//   }
//   {
//      vector<UInt> ids = pm.getIds(Condition::sma);
//      addBC(mesh, pm, mesh.elem_.getSurfacesWithMatId(ids));
//   }
//   {
//      vector<UInt> ids = pm.getIds(Condition::sibc);
//      addBC(mesh, pm, mesh.elem_.getSurfacesWithMatId(ids));
//   }
//}
//
//void
//BCGroup::buildEMSourceBC(
//      const MeshVolume& mesh,
//      const EMSourceGroup& em) {
//   vector<pair<const Volume*, UInt> > border;
//   for (UInt i = 0; i < em.count(); i++) {
//      const EMSource* source = em.get(i);
//      const BoxD3* bound = source->getBound();
//      vector<UInt> elems;
//      if (bound == NULL) {
//         elems = source->getElem();
//      } else {
//         elems = mesh.getIdsInsideBound(*bound, Element::volume);
//      }
//      border = mesh.getInternalBorder(elems);
//      for (UInt j = 0; j < border.size(); j++) {
//         bc_.insert(new BoundaryCondition(border[j], source));
//      }
//   }
//}
//
//vector<const BoundaryCondition*>
//BCGroup::get(const Condition::Type type) const {
//   vector<const BoundaryCondition*> res;
//   set<BoundaryCondition*>::iterator it;
//   for (it = bc_.begin(); it != bc_.end(); ++it) {
//      const BoundaryCondition* bc = *it;
//      if (bc->getCondition()->getConditionType() == type) {
//         res.push_back(bc);
//      }
//   }
//   return res;
//}
//
//vector<const BoundaryCondition*>
//BCGroup::getMatId(const UInt matId) const {
//   vector<const BoundaryCondition*> res;
//   set<BoundaryCondition*>::iterator it;
//   for (it = bc_.begin(); it != bc_.end(); ++it) {
//      const BoundaryCondition* bc = *it;
//      if (bc->getCondition()->getId() == matId) {
//         res.push_back(bc);
//      }
//   }
//   return res;
//}
//
//void
//BCGroup::printInfo() const {
//   cout << "--- BCGroup info ---" << endl;
//   cout << "--- End of BCGroup info ---" << endl;
//}
//
