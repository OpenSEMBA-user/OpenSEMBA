
#include "ConnectivitiesTest.h"

TEST_F(GeometryGraphConnectivitiesTest, connectivtiesReciprocity) {
    //  Creates reciprocal connectivities.
    {
        Graph::Connectivities conn(elem_);
        EXPECT_EQ(6, conn.size());
        EXPECT_TRUE(conn.existsReciprocity());
    }
    // Creates non recicprocal connectivities.
    const CoordR3* v[4] = {
            cG_.getId(CoordId(1)), cG_.getId(CoordId(2)),
            cG_.getId(CoordId(4)), cG_.getId(CoordId(5))};
    elem_.add(new Tet4(ElemId(7), v));
    {
        Graph::Connectivities conn(elem_);
        EXPECT_EQ(7, conn.size());
        EXPECT_FALSE(conn.existsReciprocity());
    }

}