#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <limits.h>
#include "ElementsGroup.h"
#include "../math/MathMatrix.h"		// Matrices class.
#include "../math/CartesianVector.h"
#ifdef USE_OPENMP
#include <omp.h>
#endif

using namespace std;

#define NUMBER_OF_VERTEX_LIMIT 2.5E6 // Max. number of vertices.
//#define MAP_DO_NOT_BUILD_SURFACE_MAPS

class Map {
public:
   Map();
   Map(const Element* elem);
	virtual ~Map();
	virtual unsigned int
	 getVolToF(unsigned int f) const = 0;
	virtual const Tet*
	 getVol(unsigned int f) const;
   virtual unsigned int
    getLocalId() const = 0;
   virtual void
    reassignPointers(const ElementsGroup& nEG) = 0;
	virtual bool
	 faceIsBoundary(unsigned int f) const;
	virtual void
	 printInfo() const = 0;
};

class MapGroup {
public:
	map<uint, Map*> maps_;
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
	pair<const Tet*, const Tet*>
	 getNeighbours(const Tri* param) const;
	bool
	 checkReciprocity() const;
};
#endif
