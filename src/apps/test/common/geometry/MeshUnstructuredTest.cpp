#include "MeshUnstructuredTest.h"

TEST_F(GeometryMeshUnstructuredTest, ctor) {
    EXPECT_EQ(mesh_.elems().size(), 2);
    EXPECT_EQ(mesh_.elems().getOf<Tetrahedron>().size(), 1);
    EXPECT_EQ(mesh_.elems().getOf<Triangle>().size(), 1);
}

TEST_F(GeometryMeshUnstructuredTest, matchingFaces) {
    const Tetrahedron4* tet = mesh_.elems().getOf<Tetrahedron4>()(0);
    vector<Face> faces;
    for (UInt f = 0; f < tet->numberOfFaces(); f++) {
        faces.push_back(Face(tet, f));
    }
    GroupElements<const SurfR> matching = mesh_.getSurfsMatching(faces);
    EXPECT_EQ(matching.size(), 1);
    const Triangle3* tri = mesh_.elems().getOf<Triangle3>()(0);
    EXPECT_EQ(*matching(0), *tri);
}

