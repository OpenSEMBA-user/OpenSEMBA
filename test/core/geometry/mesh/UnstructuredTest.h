
#pragma once

#include "MeshTest.h"
#include "geometry/mesh/Unstructured.h"

class GeometryMeshUnstructuredTest : public ::testing::Test,
                                     public GeometryMeshTest {
public:
    void SetUp() {
        GeometryMeshTest::SetUp();
        mesh_ = Mesh::Unstructured(cG_, eG_, lG_);
    }

protected:
    Mesh::Unstructured mesh_;
};

