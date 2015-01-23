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
   map<uint, Map*> tri_, tet_;
   MapGroup();
   virtual ~MapGroup();
   void
    build(const CoordinateGroup& cG, const ElementsGroup& elem);
   const Map*
    getPtrToLocalId(unsigned int id) const;
   const Map*
    getPtrToNeighMap(const Map*, unsigned int f) const;
   void
    reassignPointers(const ElementsGroup& nEG);
   void
    clear();
private:
//   pair<const Tet*, const Tet*>
//    getNeighbours(const Tri* param) const;
//   bool
//    checkReciprocity() const;
};


#endif
