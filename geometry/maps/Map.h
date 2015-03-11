#ifndef MAP_H_
#define MAP_H_

#include "../elements/ElementsGroup.h"
#ifdef USE_OPENMP
#include <omp.h>
#endif

using namespace std;

#define NUMBER_OF_VERTEX_LIMIT 2.5E6 // Max. number of vertices.
//#define MAP_DO_NOT_BUILD_SURFACE_MAPS

class Map {
public:
   Map();
   Map(const ElemR* elem);
	virtual ~Map();
	virtual UInt
	 getVolToF(UInt f) const = 0;
   virtual UInt
    getLocalId() const = 0;
   virtual void
    reassignPointers(const ElementsGroup<>& nEG) = 0;
	virtual void
	 printInfo() const = 0;
};

#endif
