#include "GroupElementsTest.h"

TEST_F(GeometryElementsGroupTest, Copy){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<>* original = new GroupCoordinates<>(coords);
    GroupCoordinates<> copied;
    copied = *original;

    EXPECT_TRUE(checkTypes(*original));
    EXPECT_TRUE(checkTypes(copied));

    delete original;

    EXPECT_TRUE(checkTypes(copied));
}

TEST_F(GeometryElementsGroupTest, CopyCtor){
    GroupCoordinates<> grp;
    {
        vector<CoordR3*> coords = newCoordR3Vector();
        grp.add(coords);
    }
    EXPECT_TRUE(checkTypes(grp));
}

TEST_F(GeometryElementsGroupTest, idsConservation){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (UInt i = 0; i < grp.size(); i++) {
        EXPECT_EQ(coords[i]->getId(), grp(i)->getId());
    }
}

