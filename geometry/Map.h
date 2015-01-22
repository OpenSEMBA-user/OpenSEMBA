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
	virtual ~Map();
	virtual const Tet*
	 getInnerTet() const = 0;
	virtual pair<const Tet*, unsigned int>
	 getInnerFace() const = 0;
	virtual pair<const Tet*, unsigned int>
	 getOuterFace() const = 0;
	virtual unsigned int
	 getLocalId() const = 0;
	virtual const Tet*
	 getLocalTet() const;
	virtual unsigned int
	 getVolToF(unsigned int f) const;
	virtual const Tet*
	 getVol(unsigned int f) const;
	virtual bool
	 isBoundary() const = 0;
   virtual void
    reassignPointers(const ElementsGroup& nEG) = 0;
	virtual bool
	 faceIsBoundary(unsigned int f) const;
	virtual void
	 printInfo() const = 0;
private:
   const Element* local;
};

class TriMap : public Map {
public:
	const Tet* vol[2];
	unsigned int volToF[2];
	TriMap();
	virtual ~TriMap();
	TriMap(
	 const Tri* localSurf, pair<const Tet*, const Tet*>);
	TriMap&
	 operator=(const TriMap& rhs);
	unsigned int
	 getVolToF(unsigned int f) const {return volToF[f];}
	pair<const Tet*, unsigned int>
	 getInnerFace() const;
	pair<const Tet*, unsigned int>
	 getOuterFace() const;
	unsigned int
	 getLocalId() const {return local->getId();}
	void
	 reassignPointers(const ElementsGroup& nEG);
	bool
	 isBoundary() const;
	void
	 printInfo() const;
};

class TetMap : public Map {
public:
	const Tet* vol[4];
	unsigned int volToF[4];
	TetMap();
	virtual ~TetMap();
	TetMap(
	 const Tet* local,
	 const Tet* neighboor[4],
	 const unsigned int neighboorFaces[4]);
	TetMap&
	 operator=(const TetMap& rhs);
	const Tet*
	 getLocalTet() const;
	unsigned int
	 getLocalId() const {return local->getId();}
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
