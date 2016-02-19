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
#include "UnstructuredTest.h"

TEST_F(GeometryMeshUnstructuredTest, ctor) {
    EXPECT_EQ(4, mesh_.coords().size());

    EXPECT_EQ(2, mesh_.elems().size());
    EXPECT_EQ(1, mesh_.elems().getOf<Tet>().size());
    EXPECT_EQ(1, mesh_.elems().getOf<Tri>().size());

    EXPECT_EQ(0, mesh_.layers().size());
}

TEST_F(GeometryMeshUnstructuredTest, matchingFaces) {
    ASSERT_GT(mesh_.elems().sizeOf<Tet4>(), 0);
    const Tet4* tet = mesh_.elems().getOf<Tet4>()(0);
    vector<Element::Face> faces;
    for (size_t f = 0; f < tet->numberOfFaces(); f++) {
        faces.push_back(Element::Face(tet, f));
    }
    Element::Group<const SurfR> matching = mesh_.getSurfsMatching(faces);
    EXPECT_EQ(matching.size(), 1);
    const Tri3* tri = mesh_.elems().getOf<Tri3>()(0);
    EXPECT_EQ(*matching(0), *tri);
}
