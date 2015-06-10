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
    MeshVolume(const MeshVolume& meshVol);
    MeshVolume(
            const GroupCoordinates&,
            const GroupElements&);
    MeshVolume(
            const GroupCoordinates& vIn,
            const GroupElements& elementIn,
            const Grid3* grid_);
    virtual ~MeshVolume();
    MeshVolume&
    operator=(const MeshVolume& param);
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
    void
    detectAndAssignPMLRegions();
    void
    createAndAssignPML(
            const PMVolumePML::Direction direction[3],
            const vector<pair<const Volume*, unsigned int> >& internalBorder,
            MeshVolume* mesh);
private:
    bool
    checkAreaCoherence() const;
};
#endif
