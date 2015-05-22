#include "GroupCVecI3FractionalUnfinishedTest.h"


void MesherFDTDMeshConfGroupCVecI3UnfinishedTest::SetUp() {
//    CVecI3 intPos(2);
//    CVecR3 relLen(0.3);
//    CVecI3Fractional fracPos(intPos, relLen);
//
//    fracPosEmpty_ = CVecI3FracU(fracPos);
//
//    vector<CoordR3*> coords;
//    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.0, 0.0, 0.0)));
//    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(1.0, 0.0, 0.0)));
//    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(0.0, 3.0, 0.0)));
//    cG_ = CoordR3Group(coords);
//
//    vector<ElemR*> elems;
//    CoordinateId vId[3] = {CoordinateId(1), CoordinateId(2), CoordinateId(3)};
//    elems.push_back(new Tri3(cG_, ElementId(1), vId));
//    eG_ = ElemRGroup(elems);
//
//    ConstElemRGroup constElemGroup = eG_;
//    fracPosWithElems_ = CVecI3FracU(fracPos, constElemGroup);
}

TEST_F(MesherFDTDMeshConfGroupCVecI3UnfinishedTest, BasicOperations){
//    EXPECT_EQ(fracPosWithElems_, fracPosWithElems_);
//    EXPECT_EQ(fracPosEmpty_, fracPosEmpty_);
//    EXPECT_NE(fracPosWithElems_, fracPosEmpty_);
//
//    CVecI3FracU copyConstructed(fracPosWithElems_);
//    EXPECT_EQ(fracPosWithElems_, copyConstructed);
//
//    CVecI3FracU assigned;
//    assigned = fracPosWithElems_;
//    EXPECT_EQ(fracPosWithElems_, assigned);

}

TEST_F(MesherFDTDMeshConfGroupCVecI3UnfinishedTest, Merging){
//    CVecI3 intPos(2);
//    CVecR3 relPos(0.3);
//    CVecI3Fractional fracPos(CVecI3(2), CVecR3(0.3));
//    CVecI3FractionalUnfinished fracPosUnf()
}
