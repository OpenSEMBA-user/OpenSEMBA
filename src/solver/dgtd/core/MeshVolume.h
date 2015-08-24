// File: Mesh.h
#ifndef MESHVOLUME_H_
#define MESHVOLUME_H_

#include "geometry/Grid.h"
#include "geometry/MeshUnstructured.h"
#include "physicalModel/PMVolumePML.h"

using namespace std;

#ifdef USE_METIS
    #include <metis.h>
    #if METIS_VER_MAJOR < 5
        #error "Mesh partitioning requires METIS version 5+"
    #endif
    #define MESH_ALLOW_PARTITIONING
#endif

class MeshVolume : public MeshUnstructured {
public:
    MeshVolume();
    MeshVolume(const MeshVolume& meshVol);
    MeshVolume(
            const CoordR3Group&,
            const ElemRGroup&);
    virtual ~MeshVolume();
    MeshVolume&
    operator=(const MeshVolume& param);
    vector<vector<ElementId>> getPartitionsIds(
            const UInt nDivisions,
            const vector<pair<ElementId,Int>> idWeights = vector<pair<ElementId,Int>>(),
            const Real* taskPower = NULL) const;
//    void detectAndAssignPMLRegions();
//    void createAndAssignPML(
//            const PMVolumePML::Direction direction[3],
//            const vector<Face>& internalBorder,
//            MeshVolume* mesh);
};

#endif
