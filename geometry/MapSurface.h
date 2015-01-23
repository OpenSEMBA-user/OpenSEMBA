/*
 * MapSurface.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPSURFACE_H_
#define SRC_COMMON_GEOMETRY_MAPSURFACE_H_

#include "Map.h"

class MapSurface: public Map {
public:
   MapSurface();
   virtual ~MapSurface();
   TriMap();
   virtual ~TriMap();
   TriMap(
         const Tri* localSurf, pair<const Tet*, const Tet*>);
   TriMap&
   operator=(const TriMap& rhs);
   unsigned int
   getLocalId() const {return local->getId();}
   unsigned int
   getVolToF(unsigned int f) const;
   pair<const Tet*, unsigned int>
   getInnerFace() const;
   pair<const Tet*, unsigned int>
   getOuterFace() const;
   void
   reassignPointers(const ElementsGroup& nEG);
   bool
   isBoundary() const;
   void
   printInfo() const;
private:
   const Tri* local;
   const Tet* vol[2];

};

#endif /* SRC_COMMON_GEOMETRY_MAPSURFACE_H_ */
