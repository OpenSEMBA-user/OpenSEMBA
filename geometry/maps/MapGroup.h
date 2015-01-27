/*
 * MapGroup.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPGROUP_H_
#define SRC_COMMON_GEOMETRY_MAPGROUP_H_

#include "MapSurface.h"
#include "MapVolume.h"
#include <vector>
#include <limits.h>

using namespace std;

class MapGroup {
public:
   MapGroup();
   virtual ~MapGroup();
   void
    build(const CoordinateGroup& cG, const ElementsGroup& elem);
   void
    reassignPointers(const ElementsGroup& nEG);
   const Tet*
    getNeighbour(const uint id, const uint face) const;
   uint
    getVolToF(const uint id, const uint face) const;
   pair<const Tet*, unsigned int>
    getNeighConnection(const uint id, const uint face) const;
   pair<const Tet*, unsigned int>
    getInnerFace(const uint id) const;
   pair<const Tet*, unsigned int>
    getOuterFace(const uint id) const;
   bool isBoundary(const uint id) const;
   bool isDomainBoundary(const uint id, const uint f) const;
   bool isDomainBoundary(pair<const Volume*, uint> boundary) const;
private:
   map<uint, MapSurface*> tri_;
   map<uint, MapVolume*>tet_;
};


#endif
