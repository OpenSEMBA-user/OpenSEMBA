/*
 * MapGroup.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPGROUP_H_
#define SRC_COMMON_GEOMETRY_MAPGROUP_H_

#include <vector>
#include <limits.h>

using namespace std;

#include "MapSurface.h"
#include "MapVolume.h"

typedef pair<const VolR*, UInt> Face;

class MapGroup {
public:
   MapGroup();
   MapGroup(const CoordinateGroup<>& cG, const ElementsGroup<>& elem);
   virtual ~MapGroup();
   void reassignPointers(const ElementsGroup<>& nEG);
   const Tet* getNeighbour(const UInt id, const UInt face) const;
   UInt getVolToF(const UInt id, const UInt face) const;
   pair<const Tet*, UInt> getNeighConnection(const UInt id, const UInt face) const;
   Face getInnerFace(const UInt id) const;
   Face getOuterFace(const UInt id) const;
   bool isBoundary(const UInt id) const;
   bool isDomainBoundary(const UInt id, const UInt f) const;
   bool isDomainBoundary(Face boundary) const;
private:
   map<UInt, MapSurface*> tri_;
   map<UInt, MapVolume*>tet_;
};


#endif
