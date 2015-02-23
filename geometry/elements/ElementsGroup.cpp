/*
 * ElementsGroup.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: luis
 */

#include "ElementsGroup.h"

ElementsGroup::ElementsGroup() {
   offsetId = 0;
   offsetIdLin = 0;
   offsetIdTri = 0;
   offsetIdTet = 0;
}

ElementsGroup::~ElementsGroup() {

}

ElementsGroup::ElementsGroup(const ElementsGroup& elem) {
    *this = elem;
}

ElementsGroup::ElementsGroup(
      const vector<Lin2>& l2,
      const vector<Tri3>& tr3, const vector<Tri6>& tr6,
      const vector<Tet4>& te4, const vector<Tet10>& te10) {
   lin2 = l2;
   tri3 = tr3;
   tri6 = tr6;
   tet4 = te4;
   tet10 = te10;
   updatePointers();
}

ElementsGroup::ElementsGroup(const vector<Lin2>& l2) {
    lin2 = l2;
    updatePointers();
}


ElementsGroup::ElementsGroup(
      const vector<Lin2>& l2,
      const vector<Tri3>& tr3, const vector<Tri6>& tr6,
      const vector<Tet4>& te4, const vector<Tet10>& te10,
      const vector<Hex8>& he8) {
   lin2 = l2;
   tri3 = tr3;
   tri6 = tr6;
   tet4 = te4;
   tet10 = te10;
   hex8 = he8;
   updatePointers();
}

ElementsGroup::ElementsGroup(
      const vector<Tri3>& tr3,
      const vector<Tri6>& tr6) {
   tri3 = tr3;
   tri6 = tr6;
   updatePointers();
}

ElementsGroup::ElementsGroup(
      const vector<Tri3>& t3,
      const vector<Quad4>& q4) {
   tri3 = t3;
   quad4 = q4;
   updatePointers();
}

ElementsGroup&
ElementsGroup::operator=(const ElementsGroup& rhs) {
   if (this == &rhs) {
      return *this;
   }
   // Copies bulk data.
   lin2 = rhs.lin2;
   tri3 = rhs.tri3;
   tri6 = rhs.tri6;
   tet4 = rhs.tet4;
   tet10 = rhs.tet10;
   hex8 = rhs.hex8;
   quad4 = rhs.quad4;
   updatePointers();
   //
   offsetId = rhs.offsetId;
   offsetIdLin = rhs.offsetIdLin;
   offsetIdTri = rhs.offsetIdTri;
   offsetIdTet = rhs.offsetIdTet;
   //
   return *this;
}

ElementsGroup&
ElementsGroup::add(
      const CoordinateGroup& coord,
      const vector<Hex8>& newHex) {
   checkIdsAreConsecutive();
   uint lastId = element[element.size()]->getId();
   hex8.reserve(hex8.size() + newHex.size());
   uint vId[8];
   for (uint i = 0; i < newHex.size(); i++) {
      // Determines coordinates ids.
      // PERFORMANCE This is O(N^2). It can be improved by creating a
      //               lexicographically sorted list of coordinates positions.
      for (uint j = 0; j < 8; j++) {
         vId[j] = newHex[i].getV(j)->getId();
      }
      uint matId = newHex[i].getMatId();
      hex8.push_back(Hex8(coord, ++lastId, matId, vId));
   }
   updatePointers();
   return *this;
}

void
ElementsGroup::reassignPointers(const CoordinateGroup& vNew) {
   for (unsigned int i = 0; i < element.size(); i++) {
      for (unsigned int j = 0; j < element[i]->numberOfCoordinates(); j++) {
         unsigned int vId = element[i]->getV(j)->getId();
         element[i]->setV(j, vNew.getPtrToId(vId));
      }
   }
}

bool
ElementsGroup::isLinear() const {
   return (tri6.size() == 0 && tet10.size() == 0);
}

Element*
ElementsGroup::getPtrToId(const unsigned int id) {
   return element[id - offsetId];
}

const Element*
ElementsGroup::getPtrToId(const unsigned int id) const {
   return element[id - offsetId];
}

const Tri*
ElementsGroup::getTriPtrToId(const unsigned int id) const {
   return tri[id - offsetIdTri];
}


vector<unsigned int>
ElementsGroup::getIdsWithMaterialId(
 const unsigned int matId) const {
    vector<unsigned int> res;
    res.reserve(element.size());
    for (unsigned int i = 0; i < element.size(); i++) {
        if (element[i]->getMatId() == matId) {
            res.push_back(element[i]->getId());
        }
    }
    return res;
}

vector<unsigned int>
ElementsGroup::getIdsWithoutMaterialId(
 const unsigned int matId) const {
    vector<unsigned int> res;
    res.reserve(element.size());
    for (unsigned int i = 0; i < element.size(); i++) {
        if (element[i]->getMatId() != matId) {
            res.push_back(element[i]->getId());
        }
    }
    return res;
}

const Tet*
ElementsGroup::getTetPtrToId(const unsigned int id) const {
   return tet[id - offsetIdTet];
}

void
ElementsGroup::printInfo() const {
   cout << "--- Elements Group info ---" << endl;
   cout << "Total number of elements: " << element.size() << endl;
   cout << endl;
   cout << "Lines: " << lineElement.size() << endl;
   cout << "lin2: " << lin2.size() << endl;
   cout << endl;
   cout << "Surfaces: " << tri.size() << endl;
   cout << "Tri3: " << tri3.size() << endl;
   cout << "Tri6: " << tri6.size() << endl;
   cout << "Quad4: " << quad4.size() << endl;
   cout << endl;
   cout << "TetVolumes: " << tet.size() << endl;
   cout << "Tet4: " << tet4.size() << endl;
   cout << "Tet10: " << tet10.size() << endl;
   cout << endl;
   cout << "Hex8: " << hex8.size() << endl;
}

unsigned int
ElementsGroup::nSurfaceElements() const {
   return tri.size();
}

unsigned int
ElementsGroup::nVolumeElements() const {
   return tet.size();
}

void
ElementsGroup::linearize() {
   if (isLinear()) {
      return;
   }
   assert(lin2.size() == 0);
   assert(tri3.size() == 0);
   assert(tet4.size() == 0);
   // Linearizes surfaces --------------------------------------------
   unsigned int nTri6 = tri6.size();
   tri3.reserve(nTri6);
   for (unsigned int i = 0; i < nTri6; i++) {
      tri3.push_back(tri6[i].linearize());
   }
   tri6.clear();
   // Linearizes volumes ---------------------------------------------
   unsigned int nTet10 = tet10.size();
   tet4.reserve(nTet10);
   for (unsigned int i = 0; i < nTet10; i++) {
      tet4.push_back(tet10[i].linearize());
   }
   tet10.clear();
   updatePointers();
}

bool
ElementsGroup::areTetrahedrons(const vector<unsigned int>& elemId) const {
   unsigned int nE = elemId.size();
   for (unsigned int i = 0; i < nE; i++) {
      const unsigned int id = elemId[i];
      const Element* e = getPtrToId(id);
      if (!e->isTet()) {
         return false;
      }
   }
   return true;
}

bool
ElementsGroup::areTriangles(const vector<unsigned int>& elemId) const {
   unsigned int nE = elemId.size();
   for (unsigned int i = 0; i < nE; i++) {
      if (!getPtrToId(elemId[i])->isTri()) {
         return false;
      }
   }
   return true;
}

vector<unsigned int>
ElementsGroup::getHexIds() const {
   const unsigned int nK = hex8.size();
   vector<unsigned int> res(nK);
   for (unsigned int i = 0; i < nK; i++) {
      res[i] = hex8[i].getId();
   }
   return res;
}

ElementsGroup
ElementsGroup::removeElementsWithMatId(
      const uint matId) const {
   //
   bool isSupported = (element.size() == tri3.size() + quad4.size());
   if (!isSupported) {
      cerr << "ERROR @ ElementsGroup: "
            << "Type of element not supported for this operation." << endl;
   }
   assert(isSupported);
   //
   vector<Tri3> tri3_;
   tri3_.reserve(tri3.size());
   for (uint i = 0; i < tri3.size(); i++) {
      if (tri3[i].getMatId() != matId) {
         tri3_.push_back(tri3[i]);
      }
   }
   vector<Quad4> quad4_;
   quad4_.reserve(quad4.size());
   for (uint i = 0; i < quad4.size(); i++) {
      if (quad4[i].getMatId() != matId) {
         quad4_.push_back(quad4[i]);
      }
   }
   return ElementsGroup(tri3_, quad4_);
}

void
ElementsGroup::checkIdsAreConsecutive() {
   unsigned int currentId = offsetId;
   for (unsigned int i = 1; i < element.size(); i++) {
      if (element[i]->getId() == currentId + 1) {
         currentId++;
      } else {
         cerr<< "ERROR @ ElementsGroup: " << endl
               << "Ids are not consecutive" << endl;
      }
   }
}

void
ElementsGroup::updatePointers() {
   element.clear();
   lineElement.clear();
   tri.clear();
   tet.clear();
   element.reserve(lin2.size() + tri3.size()
         + tri6.size() + tet4.size() + tet10.size()
         + quad4.size() + hex8.size());
   tet.reserve(tet4.size() + tet10.size());
   for (unsigned int i = 0; i < tet4.size(); i++) {
      tet.push_back(&tet4[i]);
      element.push_back(&tet4[i]);
   }
   for (unsigned int i = 0; i < tet10.size(); i++) {
      tet.push_back(&tet10[i]);
      element.push_back(&tet10[i]);
   }
   tri.reserve(tri3.size() + tri6.size());
   for (unsigned int i = 0; i < tri3.size(); i++) {
      tri.push_back(&tri3[i]);
      element.push_back(&tri3[i]);
   }
   for (unsigned int i = 0; i < tri6.size(); i++) {
      tri.push_back(&tri6[i]);
      element.push_back(&tri6[i]);
   }
   // --- **
   for (unsigned int i = 0; i < hex8.size(); i++) {
      element.push_back(&hex8[i]);
   }
   for (unsigned int i = 0; i < quad4.size(); i++) {
      element.push_back(&quad4[i]);
   }
   // --- **
   lineElement.reserve(lin2.size());
   for (unsigned int i = 0; i < lin2.size(); i++) {
      lineElement.push_back(&lin2[i]);
      element.push_back(&lin2[i]);
   }
   // Updates offsets.
   if (element.size()) {
      offsetId = element[0]->getId();
   }
   if (lineElement.size() > 0) {
      offsetIdLin = lineElement[0]->getId();
   }
   if (tri.size() > 0) {
      offsetIdTri = tri[0]->getId();
   }
   if (tet.size() > 0) {
      offsetIdTet = tet[0]->getId();
   }
}

vector<uint>
ElementsGroup::getIds(const vector<const Element*>& list) const {
   vector<uint> res;
   res.reserve(list.size());
   for (uint i = 0; i < list.size(); i++) {
      res.push_back(list[i]->getId());
   }
   return res;
}

vector<const Element*>
ElementsGroup::getElementsWithMatId(const vector<uint>& matId) const {
   vector<const Element*> res;
   res.reserve(element.size());
   for (uint m = 0; m < matId.size(); m++) {
      for (uint i = 0; i < element.size(); i++) {
         if (element[i]->getMatId() == matId[m]) {
            res.push_back(element[i]);
         }
      }
   }
   return res;
}

vector<const Element*>
ElementsGroup::getElementsWithMatLayId(
      const uint matId, const uint layId) const {
   vector<const Element*> res;
   res.reserve(element.size());
   for (uint i = 0; i < element.size(); i++) {
      if ((element[i]->getMatId() == matId)
            && (element[i]->getLayerId() == layId)) {
         res.push_back(element[i]);
      }
   }
   return res;
}

map<uint, vector<const Element*> >
ElementsGroup::separateLayers(vector<const Element*>& el) const {
   map<uint, vector<const Element*> > res;
   for (uint i = 0; i < el.size(); i++) {
      const uint layerId = el[i]->getLayerId();
      map<uint, vector<const Element*> >::iterator it = res.find(layerId);
      if (it == res.end()) {
         pair<uint, vector<const Element*> > newEntry;
         newEntry.first = layerId;
         newEntry.second.push_back(el[i]);
         res.insert(newEntry);
      } else {
         it->second.push_back(el[i]);
      }
   }
   return res;
}

vector<const Surface*>
ElementsGroup::getSurfacesWithMatId(
      const vector<uint>& matId) const {
   vector<const Surface*> res;
   res.reserve(tri.size());
   for (uint m = 0; m < matId.size(); m++) {
      for (uint i = 0; i < tri.size(); i++) {
         if (tri[i]->getMatId() == matId[m]) {
            res.push_back(tri[i]);
         }
      }
   }
   return res;
}

vector<const Element*> ElementsGroup::get(
        const Element::Type& type) const {
    vector<const Element*> res;
    res.reserve(element.size());
    for (uint i = 0; i < element.size(); i++) {
        bool matches = true;
        matches &= element[i]->getType() == type;
        if (matches) {
            res.push_back(element[i]);
        }
    }
    return res;
}

vector<const Element*> ElementsGroup::get(
        const Element::Type& type,
        const uint matId,
        const uint layerId) const {
    vector<const Element*> res;
    res.reserve(element.size());
    for (uint i = 0; i < element.size(); i++) {
        bool matches = true;
        matches &= element[i]->getType() == type;
        matches &= element[i]->getMatId() == matId;
        matches &= element[i]->getLayerId() == layerId;
        if (matches) {
            res.push_back(element[i]);
        }
    }
    return res;
}
