

#pragma once

#include "gtest/gtest.h"
#include "geometry/layer/Group.h"

class IdentifiableTest : public ::testing::Test {
public:
    IdentifiableTest() {};
    virtual ~IdentifiableTest() {};

protected:
    std::vector<SEMBA::Geometry::Layer::Layer*> newLayersVector() const;

    void areEqual(
            const std::vector<SEMBA::Geometry::Layer::Layer*>& vec,
            const SEMBA::Geometry::Layer::Group<>& layers);
};




