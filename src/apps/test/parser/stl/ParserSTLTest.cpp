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
#include "ParserSTLTest.h"

TEST_F(ParserSTLTest, Single) {
    string project = "single";
    SmbData* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
    MeshUnstructured* mesh = smb->mesh->castTo<MeshUnstructured>();
    if (smb->mesh != NULL) {
        EXPECT_EQ(mesh->coords().size(), 3);
        EXPECT_EQ(mesh->elems().getOf<Triangle3>().size(), 1);
    }
}

TEST_F(ParserSTLTest, B2) {
    string project = "B2";
    SmbData* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
    MeshUnstructured* mesh = smb->mesh->castTo<MeshUnstructured>();
    if (smb->mesh != NULL) {
        EXPECT_EQ(mesh->coords().size(), 1956);
        EXPECT_EQ(mesh->elems().getOf<Triangle3>().size(), 652);
    }
}

TEST_P(ParserSTLTest, ReadsSomething) {
    string project = GetParam();
    SmbData* smb = parseFromSTL(project);
//    ExporterVTK(smb, stlFolder_+project);
    EXPECT_TRUE(smb->mesh != NULL);
}


INSTANTIATE_TEST_CASE_P(
        stls, ParserSTLTest, ::testing::Values(
                "single",
                "B2",
                "ev55",
                "scrub2",
                "dmcwf"
        ));
