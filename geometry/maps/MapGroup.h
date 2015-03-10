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
    build(const CoordinateGroup<>& cG, const ElementsGroup<>& elem);
   void
    reassignPointers(const ElementsGroup<>& nEG);
   const Tet*
    getNeighbour(const UInt id, const UInt face) const;
   UInt
    getVolToF(const UInt id, const UInt face) const;
   pair<const Tet*, UInt>
    getNeighConnection(const UInt id, const UInt face) const;
   pair<const Volume*, UInt>
    getInnerFace(const UInt id) const;
   pair<const Volume*, UInt>
    getOuterFace(const UInt id) const;
   bool isBoundary(const UInt id) const;
   bool isDomainBoundary(const UInt id, const UInt f) const;
   bool isDomainBoundary(pair<const Volume*, UInt> boundary) const;
private:
   map<UInt, MapSurface*> tri_;
   map<UInt, MapVolume*>tet_;
};


#endif
