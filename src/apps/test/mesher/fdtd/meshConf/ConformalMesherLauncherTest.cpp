#include "ConformalMesherLauncherTest.h"

TEST_P(ConformalMesherLauncherTest, Structured_1kCell){
    const string project = GetParam();
    CVecI3 nCells(10,20,20);
    // Runs meshConf.
    OptionsMesher* opts = new OptionsMesher();
    runConformalMesher(project, nCells, opts);
    // Runs ugrMesher.
    const string file = stlFolder_ + project;
    string basicArgs = "-i " + file + ".nfde -s " + file + ".stl -o " + file;
    string args = basicArgs + " --structured";
    ugrMesher_.exec(args);
    // Makes comparisons.
    ProjectFile cmshBase(file + "_str.cmsh");
    ProjectFile cmshNew(file + ".cmsh");
    compare(cmshBase, cmshNew);
}

INSTANTIATE_TEST_CASE_P(
        stls, ConformalMesherLauncherTest, ::testing::Values(
                "single",
                "B2",
                "dmcwf"
        ));

