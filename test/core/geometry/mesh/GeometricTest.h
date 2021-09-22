
#pragma once

#include "MeshTest.h"
#include "geometry/mesh/Geometric.h"

class GeometryMeshGeometricTest : public ::testing::Test,
                                  public GeometryMeshTest {
public:
    void SetUp() {
        Grid3 grid;
        GeometryMeshTest::SetUp();
        mesh_ = Mesh::Geometric(grid, cG_, eG_, lG_);
    }

protected:
    Mesh::Geometric mesh_;
};

