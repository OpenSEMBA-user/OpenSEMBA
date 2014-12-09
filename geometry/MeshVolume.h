// File: Mesh.h
/* =============== Mesh procedures ====================================
 * A Mesh is composed of three things:
 * - A Set of Coordinates.
 * - A Group of Elements with pointers to coordinates.
 * - A Group of Maps indicating the element neighbourhood.
   ================================================================= */
#ifndef MESHVOLUME_H_
#define MESHVOLUME_H_

#include "Mesh.h"
#include "RectilinearGrid.h"

//#define MESH_USE_OLD_ALGORITHM_TO_GETINTERNAL_BORDER

#ifndef MESH_ERROR
	#define MESH_ERROR 77356
#endif

#ifdef USE_METIS
	#include <metis.h>
	#if METIS_VER_MAJOR < 5
		#error "Mesh partitioning requires METIS version 5+"
	#endif
	#define MESH_ALLOW_PARTITIONING
#endif

using namespace std;

class MeshVolume : public Mesh {
public:
	MeshVolume();
	MeshVolume(
	 const CoordinateGroup&,
	 const ElementsGroup&);
	MeshVolume(
	 const CoordinateGroup& vIn,
	 const ElementsGroup& elementIn,
	 const RectilinearGrid* grid_);
	virtual ~MeshVolume();
	MeshVolume&
	 operator=(const MeshVolume& param);
	vector<Tri3>
	 getTriWithMatId(const uint matId, const bool ignoreTet = false) const;
	vector<Tri3>
	 getTriWithId(const vector<uint>& Id) const;
	const Element*
	 getElementWithId(unsigned int id) const;
	vector<Hex8>
	 getRectilinearHexesInsideRegion(
	  const vector<const Element*>& region) const;
	pair<const Tet*, unsigned int>
	 getTetWithLocalSurf(const Element*) const;
	vector<pair<const Tet*, unsigned int> >
	 getInternalBorder(const vector<unsigned int>& elemIds) const;
	vector<pair<const Tet*, unsigned int> >
	 getExternalBorder(const vector<unsigned int>& elemIds) const;
	vector<unsigned int>
	 getAdjacentElements(const vector<unsigned int>& elemIds) const;
	vector<pair<const Tet*, unsigned int> >
	 getBorderWithNormal(
	  const vector<pair<const Tet*, unsigned int> >& border,
	  const CartesianVector<double,3>& normal);
	unsigned int
	 nVolumeElements() const;
	pair<const Tet*, unsigned int>
	 getNeighConnection(pair<const Tet*, const unsigned int>) const;
	vector<vector<unsigned int> >
	 getPartitionsIds(
	  const unsigned int nDivisions) const;
	vector<vector<unsigned int> >
	 getPartitionsIds(
	  const unsigned int nDivisions,
	  const vector<pair<unsigned int,int> > idWeights) const;
	vector<vector<unsigned int> >
	 getPartitionsIds(
	  const unsigned int nDivisions,
	  const vector<pair<unsigned int,int> > idWeights,
	  const double* taskPower) const;
	vector<unsigned int>
	 getTetIds(const vector<unsigned int> elemIds) const;
	vector<unsigned int>
	 getIdsOfCurvedTets() const;
	bool
	 isFloatingCoordinate(const CoordD3* coordinate) const;
	virtual void
	 printInfo() const;
private:
	vector<pair<const Tet*, unsigned int> >
	 getInternalBorderOfTetRegion(
	  const vector<unsigned int>& region) const;
	vector<pair<const Tet*, unsigned int> >
	 getInternalBorderOfTriRegion(
	  const vector<unsigned int>& region) const;
	void
	 check() const;
	bool
	 checkAreaCoherence() const;
	void
	 build(const CoordinateGroup& vIn, const ElementsGroup& elementIn);
};
#endif
