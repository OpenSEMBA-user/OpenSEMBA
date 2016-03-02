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
#include "ParserTest.h"

using namespace std;
using namespace SEMBA;
using namespace Parser;

Data* ParserGiDParserTest::newSmb(const string project) {
    const string testFolder("./testData/");
    const string testFile = testFolder + project + ".gid/" + project + ".smb";
    string fullPath((const char*) realpath(testFile.c_str(), NULL));
    GiD::Parser parser(fullPath);
    EXPECT_TRUE(parser.canOpen());
    Data* res = parser.read();
    EXPECT_TRUE(res != NULL);
    if (res != NULL) {
        EXPECT_TRUE(res->check());
    }
    return res;
}

TEST_F(ParserGiDParserTest, sphere) {
    Data* smb = newSmb("sphere");
    EXPECT_EQ(smb->outputRequests->getOf<OutRqNode>().size(), 3);
    EXPECT_EQ(smb->outputRequests->getOf<OutRqSurface>().size(), 1);
    EXPECT_EQ(smb->outputRequests->getOf<OutRqVolume>().size(), 2);
    EXPECT_EQ(smb->sources->size(), 1);

    const Geometry::Mesh::Geometric* mesh;
    EXPECT_NO_THROW(mesh = smb->mesh->castTo<Geometry::Mesh::Geometric>());
    const Geometry::CoordR3* coord;
    EXPECT_NO_THROW(coord =mesh->coords().getId(Geometry::CoordId(1)));
    EXPECT_EQ(Math::CVecR3(-1.8e-3), coord->pos());

    Solver::Settings settings;
    EXPECT_NO_THROW(settings = smb->solver->getSettings());

    // Checks aplication scaling factor in grid and boundary padding.
    Geometry::Grid3 grid;
    EXPECT_NO_THROW(grid =
            smb->mesh->castTo<Geometry::Mesh::Geometric>()->grid());
    EXPECT_EQ(Math::CVecR3(-2.1e-3), grid.getFullDomainBoundingBox().getMin());


    delete smb;
}
