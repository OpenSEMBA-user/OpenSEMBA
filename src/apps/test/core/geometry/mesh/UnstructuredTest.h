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
#include "geometry/mesh/Unstructured.h"
#include "geometry/element/Tetrahedron4.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;
using namespace Math;

class GeometryMeshUnstructuredTest : public ::testing::Test {
    void SetUp() {
        vector<CoordR3*> coord;
        CoordId coordId(1);
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 0.0, 0.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 0.0, 1.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(0.0, 1.0, 0.0)));
        coord.push_back(new CoordR3(coordId++, CVecR3(1.0, 0.0, 0.0)));
        CoordR3Group cG(coord);

        vector<ElemR*> elems;
        const CoordR3* vTet[4] = {
                cG.getId(CoordId(1)),
                cG.getId(CoordId(2)),
                cG.getId(CoordId(3)),
                cG.getId(CoordId(4))
        };
        const CoordR3* vTri[3] = {
                cG.getId(CoordId(2)),
                cG.getId(CoordId(1)),
                cG.getId(CoordId(3))
        };
        elems.push_back(new Tet4(ElemId(1), vTet));
        elems.push_back(new Tri3(ElemId(2), vTri));
        ElemRGroup eG(elems);

        Layer::Group<> lG;
        mesh_ = Mesh::Unstructured(cG, eG, lG);
    }

protected:
    Mesh::Unstructured mesh_;
};

#endif
