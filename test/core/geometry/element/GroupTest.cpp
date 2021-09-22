
#include "GroupTest.h"

TEST_F(GeometryElementGroupTest, Copy){
    vector<CoordR3*> coords = newCoordR3Vector();
    Coordinate::Group<>* original = new Coordinate::Group<>(coords);
    Coordinate::Group<> copied;
    copied = *original;

    EXPECT_TRUE(checkTypes(*original));
    EXPECT_TRUE(checkTypes(copied));

    delete original;

    EXPECT_TRUE(checkTypes(copied));
}

TEST_F(GeometryElementGroupTest, CopyCtor){
    Coordinate::Group<> grp;
    {
        vector<CoordR3*> coords = newCoordR3Vector();
        grp.add(coords);
    }
    EXPECT_TRUE(checkTypes(grp));
}

TEST_F(GeometryElementGroupTest, idsConservation){
    vector<CoordR3*> coords = newCoordR3Vector();
    Coordinate::Group<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (size_t i = 0; i < grp.size(); i++) {
        EXPECT_EQ(coords[i]->getId(), grp(i)->getId());
    }
}

