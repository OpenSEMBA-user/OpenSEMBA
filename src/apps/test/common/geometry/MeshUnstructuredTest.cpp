#include "MeshUnstructuredTest.h"

TEST_F(GeometryMeshUnstructuredTest, ctor) {
    EXPECT_EQ(mesh_.elems().size(), 2);
    EXPECT_EQ(mesh_.elems().getGroupOf<Tet>().size(), 1);
    EXPECT_EQ(mesh_.elems().getGroupOf<Tri>().size(), 1);
}

TEST_F(GeometryMeshUnstructuredTest, matchingFaces) {
    const Tet4* tet = mesh_.elems().getGroupOf<Tet4>()(0);
    vector<Face> faces;
    for (UInt f = 0; f < tet->numberOfFaces(); f++) {
        faces.push_back(Face(tet, f));
    }
    GroupElements<const SurfR> matching = mesh_.getSurfsMatching(faces);
    EXPECT_EQ(matching.size(), 1);
    const Tri3* tri = mesh_.elems().getGroupOf<Tri3>()(0);
    EXPECT_EQ(*matching(0), *tri);
}

