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
#include "ParserGiDParserTest.h"

using namespace std;
using namespace SEMBA;
using namespace Parser;

Data* ParserGiDParserTest::newSmb(const string project) {
    const string testFolder("./testData/");
    GiD::Parser parser(testFolder + project + ".gid/" + project + ".smb");
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
    delete smb;
}
