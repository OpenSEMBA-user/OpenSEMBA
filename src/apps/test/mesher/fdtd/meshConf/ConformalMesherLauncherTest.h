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
        while(!fileBase.eof()&&!fileNew.eof()){
            string lineBase, lineNew;
            if(!fileBase.eof()){
                lineBase = nextLine(fileBase);
            }
            if(!fileNew.eof()){
                lineNew = nextLine(fileNew);
            }
            EXPECT_TRUE(lineBase==lineNew);
        }

        if(!fileBase.eof()){
            nextLine (fileBase);
            EXPECT_TRUE(fileBase.eof());
        }

        if(!fileBase.eof()){
            nextLine (fileNew);
            EXPECT_TRUE(fileNew.eof());
        }
    }

private:
    string nextLine (ifstream& file) const {
        if(!file.eof()){
            string line;
            file >> line;
            while (!file.eof()&&isCommentLine (line)) {
                file >> line;
            }
            return line;
        }
        return "";
    }

    static bool isCommentLine (string& line){
        string trimLine = Parser::trim(line);
        return ( trimLine.empty()   ||
                (trimLine.size()==0)||
                (trimLine.substr(0,0) == string("*") ));
    }

};

#endif
