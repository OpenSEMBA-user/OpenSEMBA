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
   TetMap(
    const Tet* local,
    const Tet* neighboor[4],
    const unsigned int neighboorFaces[4]);
   TetMap&
    operator=(const TetMap& rhs);
   unsigned int
    getLocalId() const {return local->getId();}
   const Tet*
    getLocalTet() const;
   unsigned int
    getVolToF(unsigned int f) const {return volToF[f];}
   void
    reassignPointers(const ElementsGroup& nEG);
   const Tet*
    getVol(unsigned int f) const {return vol[f];}
   bool
    faceIsBoundary(unsigned int f) const;
   void
    printInfo() const;
private:
   const Tet* local;
   const Tet* vol[4];
   unsigned int volToF[4];
};

#endif /* SRC_COMMON_GEOMETRY_MAPVOLUME_H_ */
