#include "AdapterFDTDTest.h"

void AdapterFDTDTest::runProject(const SmbData* smb) const {
    SmbData* nfde = new SmbData();
    {
        ExporterGiD outGiDSmb(smb, smb->getOutputFilename() + ".smb");
    }
    AdapterFDTD(*smb).convert(*nfde);
//    compare(smb, nfde);
    {
        ExporterGiD outGiD(nfde);
        ExporterNFDE outNFDE(*nfde);
    }
    delete nfde;
}

void AdapterFDTDTest::compare(
        const SmbData* smb,
        const SmbData* nfde) const {
    EXPECT_TRUE(smb->emSources->isSimilar(*nfde->emSources));
    if (!smb->emSources->isSimilar(*nfde->emSources)) {
        smb->emSources->printInfo();
        nfde->emSources->printInfo();
    }
    EXPECT_TRUE(smb->outputRequests->isSimilar(*nfde->outputRequests));
}

TEST_P(AdapterFDTDTest,OpenFOAMConversion){
    SmbData* smb = newSmb(GetParam());
    smb->mesherOptions->setMesher(OptionsMesher::openfoam);
    runProject(smb);
    delete smb;
}

TEST_P(AdapterFDTDTest,UGRMesherConversion){
    SmbData* smb = newSmb(GetParam());
    runProject(smb);
    delete smb;
}

INSTANTIATE_TEST_CASE_P(
        Projects,
        AdapterFDTDTest,
        ::testing::Values(
                "planewave",
                "sphere",
                "rcs_1m",
                "dmcwf",
                "table"
        ));
