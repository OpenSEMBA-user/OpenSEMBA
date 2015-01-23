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
   MapSurface(
         const Tri* localSurf, pair<const Tet*, const Tet*>);
   MapSurface&
   operator=(const MapSurface& rhs);
   unsigned int
   getLocalId() const {return local->getId();}
   unsigned int
    getVolToF(unsigned int f) const;
   void
    reassignPointers(const ElementsGroup& nEG);
   bool
    isBoundary() const;
   void
    printInfo() const;
private:
   const Tri* local;
   const Tet* vol[2];
   uint volToF[2];
};

#endif /* SRC_COMMON_GEOMETRY_MAPSURFACE_H_ */
