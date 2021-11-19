#include "MeshTest.h"
#include "geometry/mesh/Geometric.h"

class MeshGeometricTest : public ::testing::Test,
    public MeshTest {
public:
    void SetUp() {
        Grid3 grid;
        MeshTest::SetUp();
        mesh_ = Mesh::Geometric(grid, cG_, eG_, lG_);
    }

protected:
    Mesh::Geometric mesh_;
};

TEST_F(MeshGeometricTest, ctor) {
    EXPECT_EQ(cG_.size(), mesh_.coords().size());

    EXPECT_EQ(eG_.size(), mesh_.elems().size());
    EXPECT_EQ(eG_.sizeOf<Tet>(), mesh_.elems().getOf<Tet>().size());
    EXPECT_EQ(eG_.sizeOf<Tri>(), mesh_.elems().getOf<Tri>().size());

    EXPECT_EQ(lG_.size(), mesh_.layers().size());
}
