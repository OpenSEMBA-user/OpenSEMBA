#include "gtest/gtest.h"
#include "mesher/fdtd/meshConf/IntersectionsGenerator.h"

class UGRMesherIntersectionsGeneratorTest : public ::testing::Test {
public:
    UGRMesherIntersectionsGeneratorTest(){
        //create grid
        CVecR3 min_  = CVecR3(0.0, 0.0, 0.0);
        CVecR3 max_  = CVecR3(5.0, 5.0, 5.0);
        CVecR3 step_ = CVecR3(1.0, 1.0, 1.0);
        grid_        = new Grid3(BoxR3(min_, max_), step_);

        //create uMesh
        vector<CoordR3*> coords;
        coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.0, 0.0, 0.0)));
        coords.push_back(new CoordR3(CoordinateId(2), CVecR3(1.0, 0.0, 0.0)));
        coords.push_back(new CoordR3(CoordinateId(3), CVecR3(0.0, 3.0, 0.0)));
        cG_ = CoordR3Group(coords);
        vector<ElemR*> elems;
        CoordinateId vId[3] = {CoordinateId(1), CoordinateId(2), CoordinateId(3)};
        elems.push_back(new Tri3(cG_, ElementId(1), vId));
        elems.push_back(new Tri3(cG_, ElementId(2), vId));
        eG_ = ElemRGroup(elems);

        uMesh_ = new MeshUnstructured (cG_, eG_);


    };

    ~UGRMesherIntersectionsGeneratorTest(){};
    const Grid3*   grid_;
    CoordR3Group   cG_;
    ElemRGroup     eG_;
    MeshUnstructured*  uMesh_;
};

TEST_F(UGRMesherIntersectionsGeneratorTest, BasicTests) {

    IntersectionsGenerator intersectionsGenerator;
    IntersectionsGroup* intersections;
    intersections = intersectionsGenerator(grid_, uMesh_);


}
