#ifndef SRC_TEST_ELEMENTSGROUPTEST_H_
#define SRC_TEST_ELEMENTSGROUPTEST_H_

#include "gtest/gtest.h"
#include "geometry/elements/GroupElements.h"

class GeometryElementsGroupTest : public ::testing::Test {

protected:
    bool checkTypes(const GroupElements<>& rhs) {
        bool res = true;
        for (UInt i = 0; i < rhs.size(); i++) {
            res &= rhs(i)->is<Elem>();
        }
        return res;
    }

    vector<CoordR3*> newCoordR3Vector() const {
        vector<CoordR3*> res;
        res.push_back(new CoordR3(CoordinateId(1), CVecR3(1.0, 1.0, 1.0)));
        res.push_back(new CoordR3(CoordinateId(2), CVecR3(2.0, 2.0, 2.0)));
        return res;
    }
};

#endif
