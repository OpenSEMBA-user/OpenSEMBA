
#pragma once

#include "gtest/gtest.h"
#include "geometry/Grid.h"

class GeometryGridTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        min_ = SEMBA::Math::CVecR3(0.0, 0.0, 0.0);
        max_ = SEMBA::Math::CVecR3(1.0, 1.0, 1.0);
        step_ = SEMBA::Math::CVecR3(0.05, 0.05, 0.05);
        grid_ = SEMBA::Geometry::Grid3(SEMBA::Geometry::BoxR3(min_, max_), step_);

        std::vector<double> offset_;
        std::vector<double> off_;
        off_.resize(3);
        SEMBA::Math::CVecR3 offsetIni;

        offsetIni = grid_.getOrigin();

    }

    SEMBA::Geometry::Grid3 grid_;
    SEMBA::Math::CVecR3 min_, max_, step_;
};


