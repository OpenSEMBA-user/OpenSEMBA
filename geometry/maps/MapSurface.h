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
   UInt
   getLocalId() const {return local->getId();}
   UInt
    getVolToF(UInt f) const;
   const Tet*
    getVol(UInt f) const {return vol[f];}
   void
    reassignPointers(const ElementsGroup<const Elem>& nEG);
   bool
    isBoundary() const;
   void
    printInfo() const;
private:
   const Tri* local;
   const Tet* vol[2];
   UInt volToF[2];
};

#endif /* SRC_COMMON_GEOMETRY_MAPSURFACE_H_ */
