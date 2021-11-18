#pragma once

#include "gtest/gtest.h"
#include "geometry/coordinate/Group.h"

class GeometryCoordinateGroupTest : public ::testing::Test {

protected:
    auto buildCoordGroup() {
        SEMBA::Geometry::CoordId id(1);
        SEMBA::Math::CVecR3 one(1.0, 1.0, 1.0);
        const size_t nCoords = 5;
        SEMBA::Geometry::CoordR3Group res;
        for (size_t i = 0; i < nCoords; i++) {
            res.add(std::make_unique<SEMBA::Geometry::CoordR3>(id++, one * (double) i));
        }
        return res;
    }

    bool checkTypes(const SEMBA::Geometry::CoordR3Group& rhs) {
        bool res = true;
        for (auto const& coord: rhs) {
            res &= coord->is<SEMBA::Geometry::Coord>();
        }
        return res;
    }
};

