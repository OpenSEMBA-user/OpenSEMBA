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
#include "MeshUnstructuredTest.h"

TEST_F(GeometryMeshUnstructuredTest, ctor) {
    EXPECT_EQ(mesh_.elems().size(), 2);
    EXPECT_EQ(mesh_.elems().getOf<Tetrahedron>().size(), 1);
    EXPECT_EQ(mesh_.elems().getOf<Triangle>().size(), 1);
}

TEST_F(GeometryMeshUnstructuredTest, matchingFaces) {
    const Tetrahedron4* tet = mesh_.elems().getOf<Tetrahedron4>()(0);
    vector<Face> faces;
    for (UInt f = 0; f < tet->numberOfFaces(); f++) {
        faces.push_back(Face(tet, f));
    }
    GroupElements<const SurfR> matching = mesh_.getSurfsMatching(faces);
    EXPECT_EQ(matching.size(), 1);
    const Triangle3* tri = mesh_.elems().getOf<Triangle3>()(0);
    EXPECT_EQ(*matching(0), *tri);
}

