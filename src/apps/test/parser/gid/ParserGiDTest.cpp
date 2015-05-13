#include "ParserGiDTest.h"

TEST_F(ParserGiDTest, sphere) {
    SmbData* smb = newSmb("sphere");
    EXPECT_EQ(smb->outputRequests->getGroupOf<OutRqNode>().size(), 3);
    EXPECT_EQ(smb->outputRequests->getGroupOf<OutRqSurface>().size(), 1);
    EXPECT_EQ(smb->outputRequests->getGroupOf<OutRqVolume>().size(), 2);
    EXPECT_EQ(smb->emSources->size(), 1);
    delete smb;
}

TEST_F(ParserGiDTest, planewave) {
    SmbData* smb = newSmb("planewave");
    EXPECT_EQ(smb->outputRequests->getGroupOf<OutRqNode>().size(), 2);
    EXPECT_EQ(smb->emSources->size(), 1);
    delete smb;
}

TEST_F(ParserGiDTest, rcs_1m) {
    SmbData* smb = newSmb("rcs_1m");
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(y,L), OptionsMesher::pec);
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(z,L), OptionsMesher::pmc);
    EXPECT_EQ(smb->outputRequests->getGroupOf<OutRqVolume>().size(), 2);
    EXPECT_EQ(smb->emSources->size(), 1);
}

TEST_F(ParserGiDTest, dmcwf) {
    SmbData* smb = newSmb("dmcwf");
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(y,L), OptionsMesher::pec);
    EXPECT_EQ(smb->mesherOptions->getBoundTermination(x,L), OptionsMesher::pmc);
    EXPECT_EQ(smb->mesherOptions->getLocationInMesh(), CVecR3(0.00485, 0.0015, 0.081));
    EXPECT_EQ(smb->outputRequests->getGroupOf<OutRqNode>().size(), 3);
    EXPECT_EQ(smb->emSources->size(), 1);
}
