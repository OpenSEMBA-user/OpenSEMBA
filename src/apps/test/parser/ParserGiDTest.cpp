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

//TEST_F(ParserGiDTest, rcs_1m) {
//   initSmb("rcs_1m");
//   EXPECT_EQ(smb_->gData->boundTermination[y].first, GlobalProblemData::pec);
//   EXPECT_EQ(smb_->gData->boundTermination[z].first, GlobalProblemData::pmc);
//   EXPECT_EQ(smb_->outputRequests->countWithType(Element::volume), 2);
//   EXPECT_EQ(smb_->emSources->count(), 1);
//}

//TEST_F(ParserGiDTest, dmcwf) {
//   initSmb("dmcwf");
//   EXPECT_EQ(smb_->gData->boundTermination[y].first, GlobalProblemData::pec);
//   EXPECT_EQ(smb_->gData->boundTermination[x].second, GlobalProblemData::pmc);
//   EXPECT_EQ(smb_->meshingParams->getLocationInMesh(), CVecR3(-0.00485, 0.0015, 0.081));
//   EXPECT_EQ(smb_->outputRequests->countWithType(Element::NODE), 3);
//   EXPECT_EQ(smb_->emSources->count(), 1);
//}
