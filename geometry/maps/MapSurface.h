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
         const Triangle* localSurf, pair<const Tetrahedron*, const Tetrahedron*>);
   MapSurface&
   operator=(const MapSurface& rhs);
   UInt
   getLocalId() const {return local->getId().toUInt();}
   UInt
    getVolToF(UInt f) const;
   const Tetrahedron*
    getVol(UInt f) const {return vol[f];}
   void
    reassignPointers(const GroupElements<const Elem>& nEG);
   bool
    isBoundary() const;
   void
    printInfo() const;
private:
   const Triangle* local;
   const Tetrahedron* vol[2];
   UInt volToF[2];
};

#endif /* SRC_COMMON_GEOMETRY_MAPSURFACE_H_ */
