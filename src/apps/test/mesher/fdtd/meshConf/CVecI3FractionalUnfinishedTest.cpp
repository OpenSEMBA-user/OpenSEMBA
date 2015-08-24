#include "CVecI3FractionalUnfinishedTest.h"

void MesherCVecI3FractionalUnfinishedTest::SetUp() {
    init();
}

void MesherCVecI3FractionalUnfinishedTest::init() {
    CVecI3 intPos(2);
    CVecR3 relLen(0.3);
    CVecI3Fractional fracPos(intPos, relLen);

    fracPosEmpty_ = CVecI3FracU(fracPos);

    vector<CoordR3*> coords;
    coords.push_back(new CoordR3(CoordinateId(1), CVecR3(0.0, 0.0, 0.0)));
    coords.push_back(new CoordR3(CoordinateId(2), CVecR3(1.0, 0.0, 0.0)));
    coords.push_back(new CoordR3(CoordinateId(3), CVecR3(0.0, 3.0, 0.0)));
    cG_ = CoordR3Group(coords);

    vector<ElemR*> elems;
    CoordinateId vId[3] = {CoordinateId(1), CoordinateId(2), CoordinateId(3)};
    elems.push_back(new Triangle3(cG_, ElementId(1), vId));
    elems.push_back(new Triangle3(cG_, ElementId(2), vId));
    eG_ = ElemRGroup(elems);

    ConstElemRGroup constElemGroup = eG_;
    fracPosWithElems_ = CVecI3FracU(fracPos, constElemGroup);
}

TEST_F(MesherCVecI3FractionalUnfinishedTest, BasicOperations) {
    EXPECT_EQ(fracPosWithElems_, fracPosWithElems_);
    EXPECT_EQ(fracPosEmpty_, fracPosEmpty_);
    EXPECT_NE(fracPosWithElems_, fracPosEmpty_);

    CVecI3FracU copyConstructed(fracPosWithElems_);
    EXPECT_EQ(2, fracPosWithElems_.size());
    EXPECT_EQ(2, copyConstructed.size());
    EXPECT_EQ(fracPosWithElems_, copyConstructed);

    const CVecI3FracU constContructed(CVecI3(2),CVecR3(0.3), eG_);
    CVecI3FracU cpyConstructedFromConst(constContructed);
    EXPECT_EQ(constContructed, cpyConstructedFromConst);

    CVecI3FracU assigned;
    assigned = fracPosWithElems_;
    EXPECT_EQ(2, fracPosWithElems_.size());
    EXPECT_EQ(2, assigned.size());
    EXPECT_EQ(fracPosWithElems_, assigned);

}

TEST_F(MesherCVecI3FractionalUnfinishedTest, Merging) {
    // Merging with fracPos with equal CVecI3Frac.
    EXPECT_EQ(fracPosEmpty_.merge(fracPosWithElems_), fracPosWithElems_);
    // Merging of two CVecI3U with different groups.
    ConstElemRGroup elems2(eG_.get(0));
    CVecI3FracU fracPosElems2(fracPosEmpty_, elems2);
    EXPECT_EQ(fracPosElems2.merge(fracPosEmpty_), fracPosElems2);
    EXPECT_EQ(fracPosElems2.merge(fracPosWithElems_), fracPosWithElems_);
    // Merging with non equal CVecI3Frac does nothing.
    CVecI3FracU fracPos2Unf(CVecI3(7), CVecR3(0.75));
    EXPECT_EQ(fracPos2Unf.merge(fracPosWithElems_), fracPos2Unf);
}
