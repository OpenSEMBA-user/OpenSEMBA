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

#include "GroupTest.h"

using namespace std;
using namespace SEMBA;
using namespace PhysicalModel;

TEST_F(PhysicalModelGroupTest, ctor) {
    PMGroup pm;
    EXPECT_NO_THROW(pm.add(
            new Volume::Classic(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0)));
    EXPECT_NO_THROW(pm.add(new Bound::PEC(Id(2))));
}

TEST_F(PhysicalModelGroupTest, bound) {
    PMGroup bounds;
    bounds.add(new Bound::PEC(Id(1)));
    bounds.add(new Bound::PMC(Id(2)));

    EXPECT_NO_THROW(bounds.get(0));
    const PhysicalModel::PhysicalModel& bound = *bounds.get(0);
    EXPECT_TRUE(bound.is<Bound::PEC>());
}
