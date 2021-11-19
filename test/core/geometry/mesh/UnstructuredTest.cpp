#include "MeshTest.h"
#include "geometry/mesh/Unstructured.h"

class MeshUnstructuredTest : public ::testing::Test,
    public MeshTest {
public:
    void SetUp() {
        MeshTest::SetUp();
        mesh_ = Mesh::Unstructured(cG_, eG_, lG_);
    }

protected:
    Mesh::Unstructured mesh_;
};
TEST_F(MeshUnstructuredTest, ctor) 
{
    EXPECT_EQ(cG_.size(), mesh_.coords().size());

    EXPECT_EQ(eG_.size(), mesh_.elems().size());
    EXPECT_EQ(eG_.sizeOf<Tet>(), mesh_.elems().getOf<Tet>().size());
    EXPECT_EQ(eG_.sizeOf<Tri>(), mesh_.elems().getOf<Tri>().size());

    EXPECT_EQ(lG_.size(), mesh_.layers().size());
}

TEST_F(MeshUnstructuredTest, copy_assignment)
{
    Mesh::Unstructured mesh = mesh_;
    for (auto const& copiedCoord : mesh.coords()) {
        auto coordPtr = mesh_.coords().getId(copiedCoord->getId());
        EXPECT_EQ(*coordPtr, copiedCoord);
    }
}

TEST_F(MeshUnstructuredTest, move_assignment)
{
    Mesh::Unstructured mesh = std::move(mesh_);
    for (auto const& copiedCoord : mesh.coords()) {
        auto coordPtr = mesh_.coords().getId(copiedCoord->getId());
        EXPECT_EQ(*coordPtr, copiedCoord);
    }
}
