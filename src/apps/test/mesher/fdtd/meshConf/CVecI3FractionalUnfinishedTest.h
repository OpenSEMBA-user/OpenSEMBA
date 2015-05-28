#ifndef MESHERCVECI3FRACTIONALUNFINISHEDTEST_H_
#define MESHERCVECI3FRACTIONALUNFINISHEDTEST_H_

#include "gtest/gtest.h"
#include "mesher/fdtd/meshConf/CVecI3FractionalUnfinished.h"

class MesherCVecI3FractionalUnfinishedTest : public ::testing::Test {
public:

    void SetUp();

protected:
    CVecI3FracU fracPosEmpty_, fracPosWithElems_;
    CoordR3Group cG_;
    ElemRGroup eG_;

    void init();
};

#endif
