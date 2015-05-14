#ifndef CONFORMALMESHERLAUNCHERTEST_H_
#define CONFORMALMESHERLAUNCHERTEST_H_

#include "gtest/gtest.h"
#include "../../../parser/stl/ParserSTLTest.h"

#include "adapter/fdtd/AdapterFDTD.h"
#include "exporter/nfde/ExporterNFDE.h"

class ConformalMesherLauncherTest : public ::ParserSTLTest {

public:

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
        while(!fileBase.eof()&&!fileNew.eof()){

            string lineBase, lineNew;
            if(!fileBase.eof()){
                lineBase = nextLine(fileBase);
                line++;
            }
            if(!fileNew.eof()){
                lineNew = nextLine(fileNew);
            }
            if (lineBase != lineNew) {
                long int iBase, iNew;
                for (UInt i = 0; i < 3; i++) {
                    fileBase >> iBase;
                    fileNew >> iNew;
                    EXPECT_EQ(iBase, iNew);
                }
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

private:
    string nextLine (ifstream& file) const {
        if(!file.eof()){
            string line;
            getline(file, line);
            while (!file.eof() && isCommentLine(line)) {
                getline(file, line);
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
