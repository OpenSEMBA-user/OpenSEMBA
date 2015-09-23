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
#include "GroupCoordinatesTest.h"

TEST_F(GeometryCoordinateGroupTest, Copy){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<>* original = new GroupCoordinates<>(coords);
    GroupCoordinates<> copied;
    copied = *original;

    EXPECT_TRUE(checkTypes(*original));
    EXPECT_TRUE(checkTypes(copied));

    delete original;

    EXPECT_TRUE(checkTypes(copied));
}

TEST_F(GeometryCoordinateGroupTest, CopyCtor){
    GroupCoordinates<> grp;
    {
        vector<CoordR3*> coords = newCoordR3Vector();
        grp.add(coords);
    }
    EXPECT_TRUE(checkTypes(grp));
}

TEST_F(GeometryCoordinateGroupTest, idsConservation){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (UInt i = 0; i < grp.size(); i++) {
        EXPECT_EQ(coords[i]->getId(), grp(i)->getId());
    }
}

TEST_F(GeometryCoordinateGroupTest, getByPos){
    vector<CoordR3*> coords = newCoordR3Vector();
    GroupCoordinates<> grp(coords);
    EXPECT_EQ(coords.size(), grp.size());
    for (UInt i = 0; i < grp.size(); i++) {
        CVecR3 pos = coords[i]->pos();
        const CoordR3* found = grp.getPos(pos);
        EXPECT_EQ(pos, found->pos());
    }
}
