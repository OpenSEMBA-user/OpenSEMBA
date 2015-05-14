#include "ConformalMesherLauncherTest.h"

TEST_F(ConformalMesherLauncherTest, Structured){
    const string project = "B2";
    const string file = stlFolder_ + project;
    CVecR3 step(0.5,0.5,0.5);
    // Runs meshConf.
    SmbData* smb = parseFromSTL(project);
    // Prepares data for mesher.
    smb->pMGroup = new GroupPhysicalModels<>();
    MatId pecId(1);
    smb->pMGroup->add(new PMPEC(pecId, "PEC"));
    smb->mesh->castTo<MeshUnstructured>()->setMatId(pecId);
    BoxR3 bound = smb->mesh->castTo<MeshUnstructured>()->getBoundingBox();
    smb->grid = new Grid3(bound, step);
    smb->mesherOptions = new OptionsMesher();
    smb->solverOptions = new OptionsSolver();
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
    // Runs ugrMesher.
    ProjectFile ugrMesher("/usr/local/bin/ugrMesher");
    string args = "-i " + file + ".nfde"
            + " -s " + file + ".stl" + " -o "
            + file + " --structured";
    ugrMesher.exec(args);
    // Makes comparisons.
    ProjectFile cmshBase(file + "_str.cmsh");
    ProjectFile cmshNew(file + ".cmsh");
    compare(cmshBase, cmshNew);
}

//INSTANTIATE_TEST_CASE_P(
//        stls, ConformalMesherLauncherTest, ::testing::Values(
//                "B2",
//                "ev55",
//                "scrub2"
//        ));

