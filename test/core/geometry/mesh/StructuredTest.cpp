#include "gtest/gtest.h"
#include "geometry/mesh/Structured.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Line2.h"

using namespace SEMBA;
using namespace Geometry;
using namespace Math;

class MeshStructuredTest : public ::testing::Test {
public:
    void SetUp() 
    {
        auto lay = lG_.add(std::make_unique<Layer::Layer>(LayerId(1), "Melon"))->get();
            
        CoordId coordId(1);
        cG_.add(std::make_unique<CoordI3>(coordId++, CVecI3(0, 0, 0)));
        cG_.add(std::make_unique<CoordI3>(coordId++, CVecI3(0, 0, 1)));
        cG_.add(std::make_unique<CoordI3>(coordId++, CVecI3(0, 1, 0)));
        cG_.add(std::make_unique<CoordI3>(coordId++, CVecI3(0, 1, 1)));

        const CoordI3* vQua[4] = {
                cG_.getId(CoordId(1)),
                cG_.getId(CoordId(2)),
                cG_.getId(CoordId(3)),
                cG_.getId(CoordId(4))
        };
        const CoordI3* vLin[2] = {
                cG_.getId(CoordId(2)),
                cG_.getId(CoordId(1))
        };

        eG_.add(std::make_unique<QuaI4>(ElemId(1), vQua, lay));
        eG_.add(std::make_unique<LinI2>(ElemId(2), vLin, lay));
        
        mesh_ = Mesh::Structured(grid_, cG_, eG_, lG_);
    }

protected:
    LayerGroup lG_;
    CoordI3Group cG_;
    ElemIGroup eG_;
    Grid3 grid_;
    Mesh::Structured mesh_;
};

TEST_F(MeshStructuredTest, ctor) 
{
    EXPECT_EQ(cG_.size(), mesh_.coords().size());

    EXPECT_EQ(eG_.size(), mesh_.elems().size());
    EXPECT_EQ(eG_.sizeOf<QuaI4>(), mesh_.elems().sizeOf<QuaI4>());
    EXPECT_EQ(eG_.sizeOf<LinI2>(), mesh_.elems().sizeOf<LinI2>());

    EXPECT_EQ(lG_.size(), mesh_.layers().size());
}

TEST_F(MeshStructuredTest, copy_assignment)
{
    Mesh::Structured mesh = mesh_;
    for (auto const& copiedCoordIt : mesh.coords()) {
        auto copiedCoord = copiedCoordIt.get();
        auto id = copiedCoord->getId();
        EXPECT_EQ(*copiedCoord, *mesh_.coords().getId(id));
    }

    EXPECT_EQ(1, mesh.layers().size());
}

TEST_F(MeshStructuredTest, move_assignment)
{
    Mesh::Structured mesh = std::move(mesh_);
    
    EXPECT_EQ(0, mesh_.coords().size());

    EXPECT_EQ(2, mesh.elems().size());
    EXPECT_EQ(0, mesh_.elems().size());
    
    EXPECT_EQ(1, mesh.layers().size());
    EXPECT_EQ(0, mesh_.layers().size());
}
