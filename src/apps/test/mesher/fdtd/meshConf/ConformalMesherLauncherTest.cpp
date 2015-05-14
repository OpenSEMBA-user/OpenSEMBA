#include "ConformalMesherLauncherTest.h"

TEST_F(ConformalMesherLauncherTest,Structured){

    // >>>>>>>>>>>>>>>>>>> runs ugrMesher <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //string args = "-auto 1 1 1 -s /home/luis/workspace/semba/projects/test/stls/B2.stl -o B2 --structured";
	string args = "-auto 1 1 1 -s ../projects/test/stls/B2.stl -o B2 --structured";

	SmbData* smb = parseFromSTL("B2");

    string project = "";
    runUGRMesher(project);

    // >>>>>>>>>>>>>>>>>>> runs meshConf <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    SmbData* smb = parseFromSTL("B2");
    ConformalMesher* conf = NULL;
    MeshStructured* meshNew_ = convertConfToMesh(conf);
    GroupPhysicalModels<PMPEC> pec = smb_->pMGroup->getGroupOf<PMPEC>();
    assert(pec.size() <= 1);
    if (pec.size() >= 1) {
        meshNew_->elems().setMatId(pec(0)->getId());
    }
    meshNew_->layers() = meshOrig_->layers().newGroup();
    LayerId ugrMesherLayerId(666);
    meshNew_->layers().add(new Layer(ugrMesherLayerId, "ugrMesher"));
    meshNew_->elems().setLayerId(ugrMesherLayerId);
    // Handles volumic materials.
    GroupPhysicalModels<PMVolume> volMat;
    volMat = smb_->pMGroup->getGroupOf<PMVolume>();
    volMat = volMat.getGroupNotOf<PMVolumePML>();
    for (UInt i = 0; i < volMat.size(); i++) {
        MatId mId = volMat(i)->getId();
        GroupElements<const VolR> vols = meshOrig_->elems().getGroupWith(mId);
        meshNew_->addAsHex(vols);
    }
    // TODO Export files.
    delete smb;

     Compare
}

//INSTANTIATE_TEST_CASE_P(
//        stls, ConformalMesherLauncherTest, ::testing::Values(
//                "B2",
//                "ev55",
//                "scrub2"
//        ));

