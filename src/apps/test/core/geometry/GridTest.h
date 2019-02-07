// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
#ifndef SRC_APPS_TEST_CORE_GEOMETRY_GRIDTEST_H_
#define SRC_APPS_TEST_CORE_GEOMETRY_GRIDTEST_H_

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


#endif /* SRC_APPS_TEST_CORE_GEOMETRY_GRIDTEST_H_ */
