#include "ConformalMesherLauncherTest.h"

void ConformalMesherLauncherTest::compare(
        ProjectFile& cmshBase,
        ProjectFile& cmshNew) const {
    ifstream fileBase, fileNew;
    cmshBase.openAsInput(fileBase);
    cmshNew.openAsInput(fileNew);
    UInt line = 0;
    while (fileBase.good() && fileNew.good()) {
        string lineBase, lineNew;
        if (!fileBase.eof()) {
            lineBase = nextLine(fileBase);
            line++;
        }
        if (!fileNew.eof()) {
            lineNew = nextLine(fileNew);
        }
        if (lineBase != lineNew) {
            CVecI3 ijkBase, ijkNew;
            fileBase >> ijkBase(x) >> ijkBase(y) >> ijkBase(z);
            fileNew >> ijkNew(x) >> ijkNew(y) >> ijkNew(z);
            EXPECT_EQ(ijkBase, ijkNew);
            string labelBase, labelNew;
            fileBase >> labelBase;
            fileNew >> labelNew;
            EXPECT_EQ(labelBase, labelNew);
            Real lengthBase, lengthNew;
            fileBase >> lengthBase;
            fileNew >> lengthNew;
            EXPECT_NEAR(lengthBase, lengthNew, 0.001);
        }
    }
}

void ConformalMesherLauncherTest::runConformalMesher(
        const string& project,
        UInt maxCellsPerLength,
        OptionsMesher* optsMesher,
        const string& subName) const {
    SmbData* smb = parseFromSTL(project);
    smb->pMGroup = new GroupPhysicalModels<>();
    MatId pecId(1);
    smb->pMGroup->add(new PMPEC(pecId, "PEC"));
    smb->mesh->castTo<MeshUnstructured>()->setMatId(pecId);
    BoxR3 bound = smb->mesh->castTo<MeshUnstructured>()->getBoundingBox();
    smb->grid = new Grid3(bound, buildStep(bound, maxCellsPerLength));
    smb->mesherOptions = optsMesher;
    smb->solverOptions = new OptionsSolver();
    smb->emSources = new GroupEMSources<>();
    smb->outputRequests = new GroupOutRqs<>();
    SmbData* nfde = new SmbData();
    AdapterFDTD(*smb).convert(*nfde);
    {
        ExporterVTK outVTKNFDE(nfde, stlFolder_ + project + subName);
        nfde->setFilename(nfde->getFilename()+subName);
        ExporterNFDE outNFDE(*nfde);
    }
    delete smb;
}


TEST_P(ConformalMesherLauncherTest, Structured_1MCell){
    const string project = GetParam();
    UInt nCells = 100;
    // Runs meshConf.
    OptionsMesher* opts = new OptionsMesher();
    runConformalMesher(project, nCells, opts, "_structured");
//    // Runs ugrMesher.
//    const string file = stlFolder_ + project;
//    string basicArgs = "-i " + file + ".nfde -s " + file + ".stl -o " + file;
//    string args = basicArgs + " --structured";
//    ugrMesher_.exec(args);
    // Makes comparisons.
//    ProjectFile cmshBase(file + "_str.cmsh");
//    ProjectFile cmshNew(file + ".cmsh");
//    compare(cmshBase, cmshNew);
}

TEST_P(ConformalMesherLauncherTest, Conformal_1MCell){
    const string project = GetParam();
    UInt nCells = 100;
    // Runs meshConf.
    OptionsMesher* opts = new OptionsMesher();
    opts->setMode(OptionsMesher::relaxed);
    runConformalMesher(project, nCells, opts, "_conformal");
//    // Runs ugrMesher.
//    const string file = stlFolder_ + project;
//    string basicArgs = "-i " + file + ".nfde -s " + file + ".stl -o " + file;
//    string args = basicArgs + " --structured";
//    ugrMesher_.exec(args);
    // Makes comparisons.
//    ProjectFile cmshBase(file + "_str.cmsh");
//    ProjectFile cmshNew(file + ".cmsh");
//    compare(cmshBase, cmshNew);
}

INSTANTIATE_TEST_CASE_P(
        stls, ConformalMesherLauncherTest, ::testing::Values(
                "single",
                "B2",
                "ev55",
                "scrub2",
                "dmcwf"
        ));

