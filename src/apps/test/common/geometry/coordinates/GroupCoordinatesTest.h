#ifndef SRC_TEST_COORDINATEGROUPTEST_H_
#define SRC_TEST_COORDINATEGROUPTEST_H_

#include "gtest/gtest.h"
#include "geometry/coordinates/GroupCoordinates.h"

class GeometryCoordinateGroupTest : public ::testing::Test {

protected:
    vector<CoordR3*> newCoordR3Vector() {
        CoordinateId id(1);
        CVecR3 one(1.0, 1.0, 1.0);
        const UInt nCoords = 5;
        vector<CoordR3*> res;
        for (UInt i = 0; i < nCoords; i++) {
            res.push_back(new CoordR3(id++, one * (Real) i));
        }
        return res;
    }

    bool checkTypes(const GroupCoordinates<>& rhs) {
        bool res = true;
        for (UInt i = 0; i < rhs.size(); i++) {
            res &= rhs(i)->is<Coord>();
        }
        return res;
    }
};

#endif
