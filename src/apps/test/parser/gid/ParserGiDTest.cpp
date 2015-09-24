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
#include "ParserGiDTest.h"

TEST_F(ParserGiDTest, sphere) {
    SmbData* smb = newSmb("sphere");
    EXPECT_EQ(smb->outputRequests->getOf<OutRqNode>().size(), 3);
    EXPECT_EQ(smb->outputRequests->getOf<OutRqSurface>().size(), 1);
    EXPECT_EQ(smb->outputRequests->getOf<OutRqVolume>().size(), 2);
    EXPECT_EQ(smb->emSources->size(), 1);
    delete smb;
}

TEST_F(ParserGiDTest, planewave) {
    SmbData* smb = newSmb("planewave");
    EXPECT_EQ(smb->outputRequests->getOf<OutRqNode>().size(), 2);
    EXPECT_EQ(smb->emSources->size(), 1);
    delete smb;
}

TEST_F(ParserGiDTest, rcs_1m) {
    SmbData* smb = newSmb("rcs_1m");
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(y,L), OptionsMesher::pec);
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(z,L), OptionsMesher::pmc);
    EXPECT_EQ(smb->outputRequests->getOf<OutRqVolume>().size(), 2);
    EXPECT_EQ(smb->emSources->size(), 1);
}

TEST_F(ParserGiDTest, dmcwf) {
    SmbData* smb = newSmb("dmcwf");
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(y,L), OptionsMesher::pec);
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(x,L), OptionsMesher::pmc);
    EXPECT_EQ(smb->mesherOptions->getLocationInMesh(), CVecR3(0.00485, 0.0015, 0.081));
    EXPECT_EQ(smb->outputRequests->getOf<OutRqNode>().size(), 3);
    EXPECT_EQ(smb->emSources->size(), 1);
}
