#ifndef CONFORMALMESHERLAUNCHERTEST_H_
#define CONFORMALMESHERLAUNCHERTEST_H_

#include "gtest/gtest.h"
#include "../../../parser/stl/ParserSTLTest.h"

#include "adapter/fdtd/AdapterFDTD.h"
#include "exporter/nfde/ExporterNFDE.h"

class ConformalMesherLauncherTest : public ::ParserSTLTest {
public:

    void SetUp() {
        ugrMesher_.setFilename("/usr/local/bin/ugrMesher");
    }

    ConformalMesherLauncherTest() {
    }

    virtual ~ConformalMesherLauncherTest() {
    }

protected:
    void compare(ProjectFile& cmshBase, ProjectFile& cmshNew) const{
        ifstream fileBase, fileNew;
        cmshBase.openAsInput(fileBase);
        cmshNew.openAsInput(fileNew);

        UInt line = 0;
        while(fileBase.good() && fileNew.good()){
            string lineBase, lineNew;
            if(!fileBase.eof()){
                lineBase = nextLine(fileBase);
                line++;
            }
            if(!fileNew.eof()){
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

    void runConformalMesher(const string& project, CVecI3& nCells,
            OptionsMesher* optsMesher) const {
        SmbData* smb = parseFromSTL(project);
        smb->pMGroup = new GroupPhysicalModels<>();
        MatId pecId(1);
        smb->pMGroup->add(new PMPEC(pecId, "PEC"));
        smb->mesh->castTo<MeshUnstructured>()->setMatId(pecId);
        BoxR3 bound = smb->mesh->castTo<MeshUnstructured>()->getBoundingBox();
        smb->grid = new Grid3(bound, nCells);
        smb->mesherOptions = optsMesher;
        smb->solverOptions = new OptionsSolver();
        smb->emSources = new GroupEMSources<>();
        smb->outputRequests = new GroupOutRqs<>();
        SmbData* nfde = new SmbData();
        AdapterFDTD(*smb).convert(*nfde);
        {
            ExporterNFDE outNFDE(*nfde);
        }
        delete smb;
    }

    ProjectFile ugrMesher_;

private:
    string nextLine (ifstream& file) const {
        if(!file.eof()){
            string line;
            getline(file, line);
            while (getline(file, line)) {
                if (!isCommentLine(line)) {
                    return line;
                }
            }
            return line;
        }
        return "";
    }

    static bool isCommentLine (string& line){
        string trimLine = Parser::trim(line);
        return ( trimLine.empty() ||
                (trimLine.size() == 0) ||
                (trimLine.substr(0,1) == string("*") ));
    }

};

#endif
