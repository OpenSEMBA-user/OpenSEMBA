#include "ConformalMesherLauncherTest.h"

TEST_F(ConformalMesherLauncherTest, Structured){
    const string project = "B2";
    CVecR3 step(1,1,1);
//    // >>>>>>>>>>>>>>>>>>> runs ugrMesher <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//    //string args = "-auto 1 1 1 -s /home/luis/workspace/semba/projects/test/stls/B2.stl -o B2 --structured";
//	string args = "-auto 1 1 1 -s ../projects/test/stls/B2.stl -o B2 --structured";
//
//	SmbData* smb = parseFromSTL("B2");
//
//    string project = "";
//    runUGRMesher(project);
//
    // >>>>>>>>>>>>>>>>>>> runs meshConf <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    SmbData* smb = parseFromSTL(project);
    // Prepares data for mesher.
    smb->pMGroup = new GroupPhysicalModels<>();
    MatId pecId(1);
    smb->pMGroup->add(new PMPEC(pecId, "PEC"));
    smb->mesh->castTo<MeshUnstructured>()->setMatId(pecId);
    BoxR3 bound = smb->mesh->castTo<MeshUnstructured>()->getBoundingBox();
    smb->grid = new Grid3(bound, step);
    smb->mesherOptions = new OptionsMesher();
    smb->emSources = new GroupEMSources<>();
    smb->outputRequests = new GroupOutRqs<>();
    // Launches adapter.
    SmbData* nfde = new SmbData();
    AdapterFDTD(*smb).convert(*nfde);
    {
        ExporterVTK outVTKSmb(smb, stlFolder_ + project + "_smb");
        ExporterVTK outVTKNFDE(nfde, stlFolder_ + project + "_nfde");
        ExporterNFDE outNFDE(*nfde);
    }
    delete smb;

}

//INSTANTIATE_TEST_CASE_P(
//        stls, ConformalMesherLauncherTest, ::testing::Values(
//                "B2",
//                "ev55",
//                "scrub2"
//        ));

