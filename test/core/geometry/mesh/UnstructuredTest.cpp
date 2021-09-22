
#include "UnstructuredTest.h"

TEST_F(GeometryMeshUnstructuredTest, ctor) {
    EXPECT_EQ(cG_.size(), mesh_.coords().size());

    EXPECT_EQ(eG_.size(), mesh_.elems().size());
    EXPECT_EQ(eG_.sizeOf<Tet>(), mesh_.elems().getOf<Tet>().size());
    EXPECT_EQ(eG_.sizeOf<Tri>(), mesh_.elems().getOf<Tri>().size());

    EXPECT_EQ(lG_.size(), mesh_.layers().size());
}

//TEST_F(GeometryMeshUnstructuredTest, matchingFaces) {
//    ASSERT_GT(mesh_.elems().sizeOf<Tet4>(), 0);
//    const Tet4* tet = mesh_.elems().getOf<Tet4>()(0);
//    vector<Element::Face> faces;
//    for (size_t f = 0; f < tet->numberOfFaces(); f++) {
//        faces.push_back(Element::Face(tet, f));
//    }
//    Element::Group<const SurfR> matching = mesh_.getSurfsMatching(faces);
//    EXPECT_EQ(matching.size(), 1);
//    const Tri3* tri = mesh_.elems().getOf<Tri3>()(0);
//    EXPECT_EQ(*matching(0), *tri);
//}
