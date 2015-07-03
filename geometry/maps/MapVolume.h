/*
 * MapVolume.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPVOLUME_H_
#define SRC_COMMON_GEOMETRY_MAPVOLUME_H_

#include "Map.h"

class MapVolume: public Map {
public:
   MapVolume();
   virtual ~MapVolume();
   MapVolume(
    const Tetrahedron* local,
    const Tetrahedron* neighboor[4],
    const UInt neighboorFaces[4]);
   MapVolume&
    operator=(const MapVolume& rhs);
   UInt
    getLocalId() const {return local->getId().toUInt();}
   const Tetrahedron*
    getLocalTet() const;
   UInt
    getVolToF(UInt f) const {return volToF[f];}
   void
    reassignPointers(const GroupElements<const Elem>& nEG);
   const Tetrahedron*
    getVol(UInt f) const {return vol[f];}
   bool
    faceIsBoundary(UInt f) const;
   void
    printInfo() const;
private:
   const Tetrahedron* local;
   const Tetrahedron* vol[4];
   UInt volToF[4];
};

#endif /* SRC_COMMON_GEOMETRY_MAPVOLUME_H_ */
