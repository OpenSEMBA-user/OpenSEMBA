
#include "CoordinateTest.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;

TEST_F(GeometryCoordinateGroupTest, Copy){
    vector<CoordR3*> coords = newCoordR3Vector();
    Coordinate::Group<>* original = new Coordinate::Group<>(coords);
    Coordinate::Group<> copied;
    copied = *original;

    EXPECT_TRUE(checkTypes(*original));
    EXPECT_TRUE(checkTypes(copied));

    delete original;

    EXPECT_TRUE(checkTypes(copied));
}

TEST_F(GeometryCoordinateGroupTest, CopyCtor){
    Coordinate::Group<> grp;
    {
        vector<CoordR3*> coords = newCoordR3Vector();
        grp.add(coords);
    }
    EXPECT_TRUE(checkTypes(grp));
}

TEST_F(GeometryCoordinateGroupTest, idsConservation){
    vector<CoordR3*> coords = newCoordR3Vector();
    Coordinate::Group<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (size_t i = 0; i < grp.size(); i++) {
        EXPECT_EQ(coords[i]->getId(), grp(i)->getId());
    }
}

TEST_F(GeometryCoordinateGroupTest, getByPos){
    vector<CoordR3*> coords = newCoordR3Vector();
    Coordinate::Group<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (size_t i = 0; i < grp.size(); i++) {
        Math::CVecR3 pos = coords[i]->pos();
        const CoordR3* found = grp.getPos(pos);
        EXPECT_EQ(pos, found->pos());
    }
}
