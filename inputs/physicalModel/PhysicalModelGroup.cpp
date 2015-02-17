/*
 * PhysicalModelGroup.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef PHYSICALMODELGROUP_H_
#include "PhysicalModelGroup.h"
#endif

PhysicalModelGroup::PhysicalModelGroup() {
   offsetId = 0;
   vacuumMat_ = NULL;
}

PhysicalModelGroup::~PhysicalModelGroup() {
   pml_.clear();
   delete vacuumMat_;
}

PhysicalModelGroup::PhysicalModelGroup(
      const vector<const PMPredefined*>& predef,
      const vector<PMVolume>& volume_,
      const vector<PMVolumeDispersive>& dispVol_,
      const vector<PMVolumePML*> pmlVol_,
      const vector<PMSurface*>& surface_) {
   vector<PMMultiport*> multiport_;
   vector<PMWire*> wire_;
   init(predef, volume_, dispVol_, pmlVol_, surface_, multiport_, wire_);
}

PhysicalModelGroup::PhysicalModelGroup(
      const vector<const PMPredefined*>& predefined,
      const vector<PMVolume>& volume,
      const vector<PMVolumeDispersive>& dispersive,
      const vector<PMVolumePML*> pml,
      const vector<PMSurface*>& surface,
      const vector<PMMultiport*> multiport,
      const vector<PMWire*> wire) {
   init(predefined, volume, dispersive, pml, surface, multiport, wire);
}

PhysicalModelGroup&
PhysicalModelGroup::operator=(const PhysicalModelGroup& rhs) {
   if (this == &rhs) {
      return *this;
   }
   volume = rhs.volume;
   dispersive = rhs.dispersive;
   pml_.resize(rhs.pml_.size(), new PMVolumePML);
   for (unsigned int i = 0; i < rhs.pml_.size(); i++) {
      pml_[i] = rhs.pml_[i];
   }
   surface = rhs.surface;
   multiport_ = rhs.multiport_;
   wire_ = rhs.wire_;
   pec = rhs.pec;
   pmc = rhs.pmc;
   sma = rhs.sma;
   updatePointers();
   offsetId = rhs.offsetId;
   return *this;
}

const PhysicalModel*
PhysicalModelGroup::getPMWithId(unsigned int id) const {
   for (unsigned int i = 0; i < count(); i++) {
      if (pm[i]->getId() == id) {
         return pm[i];
      }
   }
   assert(false);
   return NULL;
}

const PMVolumePML*
PhysicalModelGroup::getPML(
      const unsigned int i) const {
   assert(i < pml_.size());
   return pml_[i];
}

const PMVolumePML*
PhysicalModelGroup::getAssignedPML(
      const unsigned int j) const {
   assert(j < countAssignedPML());
   unsigned int pos = j;
   for (unsigned int i = 0; i < countPML(); i++) {
      if (pml_[i]->getOrientation() == PMVolumePML::undefined) {
         pos++;
      } else {
         return pml_[pos];
      }
   }
   cerr<< "ERROR @ Physical Model Group" << endl;
   cerr<< "Unable to find Asigned PML " << j << endl;
   assert(false);
   exit(-1);
}

const PMVolume*
PhysicalModelGroup::getPMVolumeWithId(unsigned int id) const {
   for (unsigned int i = 0; i < volume.size(); i++) {
      if (id == volume[i].getId()) {
         return &volume[i];
      }
   }
   for (unsigned int i = 0; i < dispersive.size(); i++) {
      if (id == dispersive[i].getId()) {
         return &dispersive[i];
      }
   }
   for (unsigned int i = 0; i < pml_.size(); i++) {
      if (id == pml_[i]->getId()) {
         return pml_[i];
      }
   }
   return getVacuum();
}

const PMVolume*
PhysicalModelGroup::getVacuum() const {
   for (unsigned int i = 0; i < volume.size(); i++) {
      if (volume[i].isVacuum()) {
         return &volume[i];
      }
   }
   return vacuumMat_;
}

const PMVolumeDispersive*
PhysicalModelGroup::getPMVolumeDispersiveWithId(unsigned int id) const {
   for (unsigned int i = 0; i < dispersive.size(); i++) {
      if (id == dispersive[i].getId()) {
         return &dispersive[i];
      }
   }
   assert(false);
   return NULL;
}

const PMSurface*
PhysicalModelGroup::getPMSurface(
      const unsigned int i) const {
   return surface[i];
}

const PMVolumePML*
PhysicalModelGroup::getPML() const {
   assert(pml_.size() != 0);
   return pml_[0];
}

void
PhysicalModelGroup::removePMLs() {
   pml_.clear();
}

void
PhysicalModelGroup::detectAndAssignPMLRegions(
      MeshVolume* mesh) {
   if (countPML() == 0) {
      return;
   }
   const unsigned int pmlId = getPML()->getId();
   vector<unsigned int> notPMLId = mesh->getIdsWithoutMaterialId(pmlId);
   vector<unsigned int> internalId = mesh->getTetIds(notPMLId);
   vector<pair<const Volume*, unsigned int> > internalBorder =
         mesh->getInternalBorder(internalId);
   // Creates PML material stretched towards +x.
   PMVolumePML::Direction direction[3];
   for (unsigned int i = 0; i < PMVolumePML::possibleDirections; i++) {
      getDirection(direction, i);
      createAndAssignPML(direction, internalBorder, mesh);
   }
}

vector<uint>
PhysicalModelGroup::getSurfaceMatIds() const {
   vector<uint> res;
   for (uint i = 0; i < count(); i++) {
      if (get(i)->isPEC() || get(i)->isPMC() || get(i)->isSurface()) {
         res.push_back(get(i)->getId());
      }
   }
   return res;
}

vector<uint>
PhysicalModelGroup::getVolumicMatIds() const {
   vector<uint> res;
   for (uint i = 0; i < count(); i++) {
      if (get(i)->isVolumic()) {
         res.push_back(get(i)->getId());
      }
   }
   return res;
}
void
PhysicalModelGroup::printInfo() const {
   cout << "---- Physical Models ----" << endl;
   cout << "Number of physical models: " << count() << endl;
   for (unsigned int i = 0; i < count(); i++) {
      getPMWithId(i + offsetId)->printInfo();
   }
}

void
PhysicalModelGroup::init(
      const vector<const PMPredefined*>& predef,
      const vector<PMVolume>& volume_,
      const vector<PMVolumeDispersive>& dispVol,
      const vector<PMVolumePML*> pml,
      const vector<PMSurface*>& surface_,
      const vector<PMMultiport*> multiport,
      const vector<PMWire*> wire) {
   volume = volume_;
   dispersive = dispVol;
   pml_ = pml;
   surface = surface_;
   multiport_ = multiport;
   wire_ = wire;
   for (unsigned int i = 0; i < predef.size(); i++) {
      if (predef[i]->isSMA()) {
         sma = *predef[i];
      } else if (predef[i]->isPEC()) {
         pec = *predef[i];
      } else if (predef[i]->isPMC()) {
         pmc = *predef[i];
      } else {
         cerr<< "ERROR @ PhysicalModelGroup: "
               << "Unable to determine predefined type." << endl;
      }
   }
   //
   offsetId = 1;
   updatePointers();
   vacuumMat_ = new PMVolume();
}

void
PhysicalModelGroup::updatePointers() {
   unsigned int lastId = 0;
   unsigned int totalSize =
         volume.size() + dispersive.size() + pml_.size() + surface.size()
         + multiport_.size() + wire_.size() + 3;
   pm.clear();
   for (unsigned int j = 0; j < totalSize; j++) {
      for (unsigned int i = 0; i < volume.size(); i++) {
         if (volume[i].getId() == lastId + 1) {
            pm.push_back(&volume[i]);
            lastId++;
         }
      }
      for (unsigned int i = 0; i < dispersive.size(); i++) {
         if (dispersive[i].getId() == lastId + 1) {
            pm.push_back(&dispersive[i]);
            lastId++;
         }
      }
      for (unsigned int i = 0; i < pml_.size(); i++) {
         if (pml_[i]->getId() == lastId + 1) {
            pm.push_back(pml_[i]);
            lastId++;
         }
      }
      for (unsigned int i = 0; i < surface.size(); i++) {
         if (surface[i]->getId() == lastId + 1) {
            pm.push_back(surface[i]);
            lastId++;
         }
      }
      for (unsigned int i = 0; i < multiport_.size(); i++) {
         if (multiport_[i]->getId() == lastId + 1) {
            pm.push_back(multiport_[i]);
            lastId++;
         }
      }
      for (unsigned int i = 0; i < wire_.size(); i++) {
         if (wire_[i]->getId() == lastId + 1) {
            pm.push_back(wire_[i]);
            lastId++;
         }
      }
      if (pec.getId() == lastId + 1) {
         pm.push_back(&pec);
         lastId++;
      }
      if (pmc.getId() == lastId + 1) {
         pm.push_back(&pmc);
         lastId++;
      }
      if (sma.getId() == lastId + 1) {
         pm.push_back(&sma);
         lastId++;
      }
   }
}

void
PhysicalModelGroup::createAndAssignPML(
      const PMVolumePML::Direction direction[3],
      const vector<pair<const Tet*, unsigned int> >& internalBorder,
      MeshVolume* mesh) {
   // Computes bound of PML pointing in this direction.
   BoxD3 bound = mesh->getBound(internalBorder);
   pair<CVecD3,CVecD3> pmlBound;
   for (unsigned int i = 0; i < 3; i++) {
      if (direction[i] == PMVolumePML::minus) {
         pmlBound.second(i) = bound.getMin()(i);
         pmlBound.first(i) = - numeric_limits<double>::infinity() ;
      } else if (direction[i] == PMVolumePML::plus) {
         pmlBound.first(i) = bound.getMax()(i);
         pmlBound.first(i) = numeric_limits<double>::infinity() ;
      } else {
         pmlBound.first(i) = bound.getMin()(i);
         pmlBound.second(i) = bound.getMax()(i);
      }
   }
   // Gets ids of PMLs inside the pml bound. Removes non-tet.
   vector<unsigned int> allPMLIds = mesh->getIdsInsideBound(pmlBound);
   vector<unsigned int> PMLIds = mesh->getTetIds(allPMLIds);
   // Creates new PML material for that bound and sets in mesh.
   if (PMLIds.size() != 0) {
      unsigned int lastId = count();
      pml_.push_back(new PMVolumePML(lastId+1, direction, mesh->getBound(PMLIds)));
      updatePointers();
      mesh->setMaterialIds(PMLIds, lastId+1);
   }
}

void
PhysicalModelGroup::getDirection(
      PMVolumePML::Direction direction[3],
      const unsigned int i) const {
   assert(i < PMVolumePML::possibleDirections);
   direction[x] = getDirectionFromInt((i/9) % 3);
   direction[y] = getDirectionFromInt((i/3) % 3);
   direction[z] = getDirectionFromInt(i % 3);
   assert( !(direction[0] == PMVolumePML::none
         && direction[1] == PMVolumePML::none
         && direction[2] == PMVolumePML::none) );
}

PMVolumePML::Direction
PhysicalModelGroup::getDirectionFromInt(
      const unsigned int i) const {
   assert(PMVolumePML::plus == 0);
   assert(PMVolumePML::minus == 1);
   assert(PMVolumePML::none == 2);
   assert(i == PMVolumePML::plus
         || i == PMVolumePML::minus
         || i == PMVolumePML::none);
   if (PMVolumePML::plus == i) {
      return PMVolumePML::plus;
   } else if (PMVolumePML::minus == i) {
      return PMVolumePML::minus;
   } else {
      return PMVolumePML::none;
   }
}

