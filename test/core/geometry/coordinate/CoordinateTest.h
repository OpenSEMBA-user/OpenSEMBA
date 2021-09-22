
#pragma once

#include "gtest/gtest.h"
#include "geometry/coordinate/Group.h"

class GeometryCoordinateGroupTest : public ::testing::Test {

protected:
    std::vector<SEMBA::Geometry::CoordR3*> newCoordR3Vector() {
        SEMBA::Geometry::CoordId id(1);
        SEMBA::Math::CVecR3 one(1.0, 1.0, 1.0);
        const size_t nCoords = 5;
        std::vector<SEMBA::Geometry::CoordR3*> res;
        for (size_t i = 0; i < nCoords; i++) {
            res.push_back(new SEMBA::Geometry::CoordR3(id++, one * (double) i));
        }
        return res;
    }

    bool checkTypes(const SEMBA::Geometry::Coordinate::Group<>& rhs) {
        bool res = true;
        for (size_t i = 0; i < rhs.size(); i++) {
            res &= rhs(i)->is<SEMBA::Geometry::Coord>();
        }
        return res;
    }
};

