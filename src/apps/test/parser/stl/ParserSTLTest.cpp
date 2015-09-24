#include "ParserSTLTest.h"

TEST_F(ParserSTLTest, Single) {
    string project = "single";
    SmbData* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
    MeshUnstructured* mesh = smb->mesh->castTo<MeshUnstructured>();
    if (smb->mesh != NULL) {
        EXPECT_EQ(mesh->coords().size(), 3);
        EXPECT_EQ(mesh->elems().getOf<Triangle3>().size(), 1);
    }
}

TEST_F(ParserSTLTest, B2) {
    string project = "B2";
    SmbData* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
    MeshUnstructured* mesh = smb->mesh->castTo<MeshUnstructured>();
    if (smb->mesh != NULL) {
        EXPECT_EQ(mesh->coords().size(), 1956);
        EXPECT_EQ(mesh->elems().getOf<Triangle3>().size(), 652);
    }
}

TEST_P(ParserSTLTest, ReadsSomething) {
    string project = GetParam();
    SmbData* smb = parseFromSTL(project);
//    ExporterVTK(smb, stlFolder_+project);
    EXPECT_TRUE(smb->mesh != NULL);
}


INSTANTIATE_TEST_CASE_P(
        stls, ParserSTLTest, ::testing::Values(
                "single",
                "B2",
                "ev55",
                "scrub2",
                "dmcwf"
        ));
