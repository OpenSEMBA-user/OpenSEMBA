
#pragma once

#include "gtest/gtest.h"
#include "geometry/element/Group.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;

class GeometryElementGroupTest : public ::testing::Test {

protected:
    bool checkTypes(const Element::Group<>& rhs) {
        bool res = true;
        for (size_t i = 0; i < rhs.size(); i++) {
            res &= rhs(i)->is<Elem>();
        }
        return res;
    }

    vector<CoordR3*> newCoordR3Vector() const {
        vector<CoordR3*> res;
        res.push_back(new CoordR3(CoordId(1), Math::CVecR3(1.0, 1.0, 1.0)));
        res.push_back(new CoordR3(CoordId(2), Math::CVecR3(2.0, 2.0, 2.0)));
        return res;
    }
};

