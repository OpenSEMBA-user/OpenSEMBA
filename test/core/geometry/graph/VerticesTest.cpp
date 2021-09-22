

#include "VerticesTest.h"

TEST_F(GeometryGraphVerticesTest, ctor) {
    //  Creates Graph.
    Graph::Vertices<ElemR,CoordR3> graph;
    graph.init(elem_);
    EXPECT_EQ(5, graph.numElems());
    EXPECT_EQ(6, graph.numBounds());
}
