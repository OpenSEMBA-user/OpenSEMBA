// File: Mesh.h
/* =============== Mesh procedures ====================================
 * A Mesh is composed of three things:
 * - A Set of Coordinates.
 * - A Group of Elements with pointers to coordinates.
 * - A Group of Maps indicating the element neighbourhood.
   ================================================================= */
#ifndef MESHVOLUME_H_
#define MESHVOLUME_H_

#include "Grid.h"
#include "Mesh.h"

//#define MESH_USE_OLD_ALGORITHM_TO_GETINTERNAL_BORDER

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
    MapGroup map;
    MeshVolume();
    MeshVolume(const MeshVolume& meshVol);
    MeshVolume(
            const CoordinateGroup&,
            const ElementsGroup&);
    MeshVolume(
            const CoordinateGroup& vIn,
            const ElementsGroup& elementIn,
            const Grid3* grid_);
    virtual ~MeshVolume();
    MeshVolume&
    operator=(const MeshVolume& param);
    vector<Tri3>
    getTriWithMatId(const uint matId, const bool ignoreTet = false) const;
    vector<Tri3> getTriWithId(const vector<uint>& Id) const;
    vector<BoxD3> getRectilinearHexesInsideRegion(
            const vector<const Element*>& region) const;
    pair<const Volume*, unsigned int>
    getBoundary(const Surface*) const;
    vector<pair<const Volume*, unsigned int> >
    getInternalBorder(const vector<unsigned int>& elemIds) const;
    vector<pair<const Volume*, unsigned int> >
    getExternalBorder(const vector<unsigned int>& elemIds) const;
    vector<unsigned int>
    getAdjacentElements(const vector<unsigned int>& elemIds) const;
    vector<pair<const Tet*, unsigned int> >
    getBorderWithNormal(
            const vector<pair<const Tet*, unsigned int> >& border,
            const CartesianVector<double,3>& normal);
    pair<const Volume*, unsigned int> getNeighConnection(
            pair<const Volume*, const unsigned int> inner) const;
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
    virtual bool
    isOnBoundary(const CVecD3 pos) const;
    virtual const CoordD3*
    getClosestVertex(const CVecD3 pos) const;
    virtual vector<const Surface*>
    getMaterialBoundary(
            const uint matId,
            const uint layId) const;
    virtual vector<BoxD3>
    discretizeWithinBoundary(
            const uint matId,
            const uint layId) const;
    void
    printInfo() const;
private:
    vector<pair<const Volume*, unsigned int> >
    getInternalBorderOfTetRegion(
            const vector<unsigned int>& region) const;
    vector<pair<const Volume*, unsigned int> >
    getInternalBorderOfTriRegion(
            const vector<unsigned int>& region) const;
    void
    check() const;
    bool
    checkAreaCoherence() const;
};

inline pair<const Volume*, unsigned int>
MeshVolume::getNeighConnection(
        pair<const Volume*, const unsigned int> inner) const {
    uint inId = inner.first->getId();
    uint inFace = inner.second;
    return map.getNeighConnection(inId, inFace);
}

#endif
