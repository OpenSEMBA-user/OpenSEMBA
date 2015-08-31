#ifndef SEMBATEST_H_
#define SEMBATEST_H_

#include "gtest/gtest.h"
#include "ProjectFile.h"
#include "geometry/MeshUnstructured.h"

class GeometryMeshUnstructuredTest : public ::testing::Test {
    void SetUp() {
        vector<CoordR3*> coord;
        CoordinateId coordId(1);
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 0.0, 0.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 0.0, 1.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 1.0, 0.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(1.0, 0.0, 0.0)));
        CoordR3Group cG(coord);
        vector<ElemR*> elems;
        CoordinateId vIdTet[4] = {
                CoordinateId(1),
                CoordinateId(2),
                CoordinateId(3),
                CoordinateId(4)};
        CoordinateId vIdTri[3] = {
                CoordinateId(2),
                CoordinateId(1),
                CoordinateId(3)};
        elems.push_back(new Tetrahedron4(cG, ElementId(1), vIdTet, LayerId(0), MatId(0)));
        elems.push_back(new Triangle3(cG, ElementId(2), vIdTri, LayerId(1), MatId(0)));
        ElemRGroup eG(elems);

        GroupLayers<> lG;
        mesh_ = MeshUnstructured(cG, eG, lG);
    }

protected:
    MeshUnstructured mesh_;
};

#endif
