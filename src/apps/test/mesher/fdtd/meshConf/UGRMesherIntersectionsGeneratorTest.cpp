#include "gtest/gtest.h"
#include "mesher/fdtd/meshConf/IntersectionsGenerator.h"

class UGRMesherIntersectionsGeneratorTest : public ::testing::Test {
public:
    UGRMesherIntersectionsGeneratorTest(){
        intersectionsGenerator_1 = new IntersectionsGenerator;
        coords.resize(3);
        coords[0](0) = 0.9; coords[0](1) = 0.9; coords[0](2) = 0.5;
        coords[1](0) = 3.1; coords[1](1) = 0.9; coords[1](2) = 0.5;
        coords[2](0) = 0.9; coords[2](1) = 3.1; coords[2](2) = 0.5;

        CVecR3 min_  = CVecR3(0.0, 0.0, 0.0);
        CVecR3 max_  = CVecR3(5.0, 5.0, 5.0);
        CVecR3 step_ = CVecR3(1.0, 1.0, 1.0);
        grid_        = new Grid3(BoxR3(min_, max_), step_);
    };

    ~UGRMesherIntersectionsGeneratorTest(){};
    const Grid3*   grid_;
    vector<CVecR3> coords;
    IntersectionsGenerator *intersectionsGenerator_1;
};

TEST_F(UGRMesherIntersectionsGeneratorTest, BasicTests) {
    EXPECT_TRUE(intersectionsGenerator_1!=NULL);
    vector<CVecR3> intersections;
    vector<CVecR3> intersectionsTrue;

    intersectionsGenerator_1 = new IntersectionsGenerator;
    intersectionsGenerator_1->grid_ = grid_;
    //intersectionsGenerator_1->addIntersection(coords, intersections);

    intersectionsGenerator_1->addIntersectionBoundig(coords, intersections);

    cout<<"joder"<<endl;
}
