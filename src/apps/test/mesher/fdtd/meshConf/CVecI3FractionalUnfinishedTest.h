#ifndef CONFORMALMESHERLAUNCHERTEST_H_
#define CONFORMALMESHERLAUNCHERTEST_H_

#include "gtest/gtest.h"
#include "mesher/fdtd/meshConf/CVecI3FractionalUnfinished.h"

class MesherCVecI3UnfinishedTest : public ::testing::Test {
public:

    void SetUp();

protected:
    CVecI3FracU fracPosEmpty_, fracPosWithElems_;
    CoordR3Group cG_;
    ElemRGroup eG_;

};

#endif
