/*
 * MeshOpenfoam.h
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#ifndef MESHOPENFOAM_H_
#define MESHOPENFOAM_H_


#include "../../common/geometry/Mesh.h"
#include "../geometry/Grid.h"
#include "OpenfoamBoundary.h"

typedef pair<uint, vector<uint> > FaceIdentifier;

class MeshOpenfoam : public Mesh {
public:
    MeshOpenfoam();
    MeshOpenfoam(
            const CoordinateGroup& cG,
            const Grid3* grid,
            const vector<FaceIdentifier>& faceIndentifiers,
            const vector<uint>& faceOwner,
            const vector<uint>& faceNeighbour,
            const vector<OpenfoamBoundary>& boundaries);
    virtual ~MeshOpenfoam();
    vector<Polyhedron> getPolyhedrons() const;
    vector<const Polygon*> getBoundaryPolygons(
            const string& boundaryName) const;
    bool isOnBoundary(const CVecD3 pos) const;
    const CoordD3* getClosestVertex(
            const CVecD3 pos) const;
    vector<const Surface*> getMaterialBoundary(
            const uint matId,
            const uint layId) const;
    vector<BoxD3> discretizeWithinBoundary(
            const uint matId,
            const uint layId) const;
    void checkAllFacesAreRectangular() const;
    bool areFacesRectangular() const;
    void printInfo() const;
private:
    vector<Polygon> face_;
    vector<uint> owner_;
    vector<uint> neighbour_;
    vector<OpenfoamBoundary> boundary_;
    const OpenfoamBoundary* getBoundary(
            const string& boundaryName) const;
    vector<BoxD3> discretizeWithinBoundary(
            const vector<const Surface*>& faces) const;
    vector<pair<const Surface*, const Surface*> >
    getPairsDefiningVolumeWithin(
            const vector<const Surface*>& boundary) const;
};

#endif /* MESHOPENFOAM_H_ */
