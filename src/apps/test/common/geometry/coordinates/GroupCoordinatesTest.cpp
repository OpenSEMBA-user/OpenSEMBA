#include "GroupCoordinatesTest.h"

TEST_F(GeometryCoordinateGroupTest, Copy){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<>* original = new GroupCoordinates<>(coords);
    GroupCoordinates<> copied;
    copied = *original;

    EXPECT_TRUE(checkTypes(*original));
    EXPECT_TRUE(checkTypes(copied));

    delete original;

    EXPECT_TRUE(checkTypes(copied));
}

TEST_F(GeometryCoordinateGroupTest, CopyCtor){
    GroupCoordinates<> grp;
    {
        vector<CoordR3*> coords = newCoordR3Vector();
        grp.add(coords);
    }
    EXPECT_TRUE(checkTypes(grp));
}

TEST_F(GeometryCoordinateGroupTest, idsConservation){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (UInt i = 0; i < grp.size(); i++) {
        EXPECT_EQ(coords[i]->getId(), grp(i)->getId());
    }
}

TEST_F(GeometryCoordinateGroupTest, getByPos){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (UInt i = 0; i < grp.size(); i++) {
        CVecR3 pos = coords[i]->pos();
        const CoordR3* found = grp.get(pos);
        EXPECT_EQ(pos, found->pos());
    }
}
