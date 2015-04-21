#include "AdapterFDTDTest.h"

//TEST_P(AdapterFDTDTest, OpenFOAMConversion){
//   SmbData* smb = newSmb(GetParam());
//   smb->mesherOptions->setMesher(OptionsMesher::openfoam);
//   runProject(smb);
//   delete smb;
//}

TEST_P(AdapterFDTDTest, ugrMesherConversion){
   SmbData* smb = newSmb(GetParam());
   runProject(smb);
   delete smb;
}

INSTANTIATE_TEST_CASE_P(
      Projects,
      AdapterFDTDTest,
      ::testing::Values(
//             "sphere",
//             "planewave",
//             "rcs_1m",
             "dmcwf"
//             "table"
            ));
