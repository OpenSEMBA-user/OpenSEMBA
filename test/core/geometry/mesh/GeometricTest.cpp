
#include "GeometricTest.h"

TEST_F(GeometryMeshGeometricTest, ctor) {
    EXPECT_EQ(cG_.size(), mesh_.coords().size());

    EXPECT_EQ(eG_.size(), mesh_.elems().size());
    EXPECT_EQ(eG_.sizeOf<Tet>(), mesh_.elems().getOf<Tet>().size());
    EXPECT_EQ(eG_.sizeOf<Tri>(), mesh_.elems().getOf<Tri>().size());

    EXPECT_EQ(lG_.size(), mesh_.layers().size());
}
