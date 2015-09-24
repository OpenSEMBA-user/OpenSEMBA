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
#ifndef SEMBATEST_H_
#define SEMBATEST_H_

#include "gtest/gtest.h"
#include "ProjectFile.h"
#include "geometry/MeshUnstructured.h"

class GeometryMeshUnstructuredTest : public ::testing::Test {
    void SetUp() {
        vector<CoordR3*> coord;
        CoordinateId coordId(1);
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 0.0, 0.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 0.0, 1.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 1.0, 0.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(1.0, 0.0, 0.0)));
        CoordR3Group cG(coord);
        vector<ElemR*> elems;
        CoordinateId vIdTet[4] = {
                CoordinateId(1),
                CoordinateId(2),
                CoordinateId(3),
                CoordinateId(4)};
        CoordinateId vIdTri[3] = {
                CoordinateId(2),
                CoordinateId(1),
                CoordinateId(3)};
        elems.push_back(new Tetrahedron4(cG, ElementId(1), vIdTet, LayerId(0), MatId(0)));
        elems.push_back(new Triangle3(cG, ElementId(2), vIdTri, LayerId(1), MatId(0)));
        ElemRGroup eG(elems);

        GroupLayers<> lG;
        mesh_ = MeshUnstructured(cG, eG, lG);
    }

protected:
    MeshUnstructured mesh_;
};

#endif
