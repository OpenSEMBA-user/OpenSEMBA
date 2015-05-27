#include "gtest/gtest.h"
#include "mesher/fdtd/meshConf/IntersectionsGenerator.h"

class MesherIntersectionsGeneratorTest : public ::testing::Test {
public:
    MesherIntersectionsGeneratorTest() {}
    ~MesherIntersectionsGeneratorTest() {};

    void init(const CVecR3 &min_,const CVecR3 &max_,
              const CVecR3 &step_, const vector<CoordR3*> coords) {

        //create grid
        grid        = new Grid3(BoxR3(min_, max_), step_);
        //create uMesh
        cG_ = CoordR3Group(coords);
        vector<ElemR*> elems;
        CoordinateId vId[3] = {CoordinateId(1), CoordinateId(2), CoordinateId(3)};
        elems.push_back(new Tri3(cG_, ElementId(1), vId));
        eG_ = ElemRGroup(elems);
        uMesh = new MeshUnstructured (cG_, eG_);
    };

    const Grid3*   grid;
    MeshUnstructured*  uMesh;

private:
    CoordR3Group   cG_;
    ElemRGroup     eG_;

};

TEST_F(MesherIntersectionsGeneratorTest, BasicTests1) {

    CVecR3 min  = CVecR3(0.0, 0.0, 0.0);
    CVecR3 max  = CVecR3(5.0, 5.0, 5.0);
    CVecR3 step = CVecR3(1.0, 1.0, 1.0);

    vector<CoordR3*> coords;
    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.5, 0.5, 0.5)));
    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(3.8, 0.5, 0.5)));
    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(0.5, 3.8, 0.5)));

    init(min, max, step, coords);

    IntersectionsGenerator intersectionsGenerator;
    IntersectionsGroup* intersections;
    intersections = intersectionsGenerator(grid, uMesh);

    IntersectionsGroup::iterator it;

    for(it=intersections->begin(); it!=intersections->end(); it++){
        if((*it)->getRangeBase()==1){
            EXPECT_EQ(0.5,(*it)->len_[z]);
        }
    }

    for(it=intersections->begin(); it!=intersections->end(); it++){
        EXPECT_NE(0,(*it)->getRangeBase());
        if((*it)->getRangeBase()==1){
            EXPECT_EQ(CartesianDirection::dirZ,(*it)->getDirBase());
        }else if((*it)->getRangeBase()==2){
            EXPECT_NE(CartesianDirection::dirZ,(*it)->getDirBase());
        }else{
            EXPECT_EQ(CartesianDirection::dirNode,(*it)->getDirBase());
        }
    }
    EXPECT_EQ(21, intersections->size());
}


TEST_F(MesherIntersectionsGeneratorTest, BasicTests2) {

    CVecR3 min  = CVecR3(0.0, 0.0, 0.0);
    CVecR3 max  = CVecR3(5.0, 5.0, 5.0);
    CVecR3 step = CVecR3(1.0, 1.0, 1.0);

    vector<CoordR3*> coords;
    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.5, 0.5, 0.5)));
    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(0.5, 3.8, 0.5)));
    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(0.5, 0.5, 3.8)));

    init(min, max, step, coords);

    IntersectionsGenerator intersectionsGenerator;
    IntersectionsGroup* intersections;
    intersections = intersectionsGenerator(grid, uMesh);

    IntersectionsGroup::iterator it;

    for(it=intersections->begin(); it!=intersections->end(); it++){
        EXPECT_EQ(0.5,(*it)->len_[x]);
    }

    for(it=intersections->begin(); it!=intersections->end(); it++){
        EXPECT_NE(0,(*it)->getRangeBase());
        if((*it)->getRangeBase()==1){
            EXPECT_EQ(CartesianDirection::dirX,(*it)->getDirBase());
        }else if((*it)->getRangeBase()==2){
            EXPECT_NE(CartesianDirection::dirX,(*it)->getDirBase());
        }else{
            EXPECT_EQ(CartesianDirection::dirNode,(*it)->getDirBase());
        }
    }

    EXPECT_EQ(21, intersections->size());
}

TEST_F(MesherIntersectionsGeneratorTest, BasicTests3) {

    CVecR3 min  = CVecR3(0.0, 0.0, 0.0);
    CVecR3 max  = CVecR3(5.0, 5.0, 5.0);
    CVecR3 step = CVecR3(1.0, 1.0, 1.0);

    vector<CoordR3*> coords;
    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.5, 0.5, 0.5)));
    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(0.5, 0.5, 3.8)));
    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(3.8, 0.5, 0.5)));

    init(min, max, step, coords);

    IntersectionsGenerator intersectionsGenerator;
    IntersectionsGroup* intersections;
    intersections = intersectionsGenerator(grid, uMesh);

    IntersectionsGroup::iterator it;

    for(it=intersections->begin(); it!=intersections->end(); it++){
        EXPECT_EQ(0.5,(*it)->len_[y]);
    }

    for(it=intersections->begin(); it!=intersections->end(); it++){
        EXPECT_NE(0,(*it)->getRangeBase());
        if((*it)->getRangeBase()==1){
            EXPECT_EQ(CartesianDirection::dirY,(*it)->getDirBase());
        }else if((*it)->getRangeBase()==2){
            EXPECT_NE(CartesianDirection::dirY,(*it)->getDirBase());
        }else{
            EXPECT_EQ(CartesianDirection::dirNode,(*it)->getDirBase());
        }
    }

    EXPECT_EQ(21, intersections->size());
}

//
//TEST_F(MesherIntersectionsGeneratorTest, BasicTests4) {
//
//    CVecR3 min  = CVecR3(0.0, 0.0, 0.0);
//    CVecR3 max  = CVecR3(5.0, 5.0, 5.0);
//    CVecR3 step = CVecR3(1.0, 1.0, 1.0);
//
//    vector<CoordR3*> coords;
//    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.5, 0.5, 0.5)));
//    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(3.5, 3.5, 0.5)));
//    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(2.0, 2.0, 1.5)));
//
//    init(min, max, step, coords);
//
//    IntersectionsGenerator intersectionsGenerator;
//    IntersectionsGroup* intersections;
//    intersections = intersectionsGenerator(grid, uMesh);
//
//    IntersectionsGroup::iterator it;
//
//    for(it=intersections->begin(); it!=intersections->end(); it++){
//        EXPECT_EQ(0.0,(*it)->len_[x]);
//        EXPECT_EQ(0.0,(*it)->len_[y]);
//        EXPECT_EQ(0.0,(*it)->len_[z]);
//        EXPECT_EQ(0,(*it)->getRangeBase());
//        EXPECT_EQ(CartesianDirection::dirNode,(*it)->getDirBase());
//    }
//
//    EXPECT_EQ(1, intersections->size());
//}
//
//
//TEST_F(MesherIntersectionsGeneratorTest, BasicTests5) {
//
//    CVecR3 min  = CVecR3(0.0, 0.0, 0.0);
//    CVecR3 max  = CVecR3(5.0, 5.0, 5.0);
//    CVecR3 step = CVecR3(1.0, 1.0, 1.0);
//
//    vector<CoordR3*> coords;
//    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(3.2, 2.5, 0.5)));
//    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(3.8, 2.5, 0.5)));
//    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(3.5, 3.5, 0.5)));
//
//    init(min, max, step, coords);
//
//    IntersectionsGenerator intersectionsGenerator;
//    IntersectionsGroup* intersections;
//    intersections = intersectionsGenerator(grid, uMesh);
//
//    EXPECT_TRUE(intersections->size()>0);
//    EXPECT_TRUE(intersections->size()==21);
//}
//






