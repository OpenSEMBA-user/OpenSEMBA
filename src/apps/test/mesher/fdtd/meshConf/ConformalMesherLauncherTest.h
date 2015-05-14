#ifndef CONFORMALMESHERLAUNCHERTEST_H_
#define CONFORMALMESHERLAUNCHERTEST_H_

#include "gtest/gtest.h"
#include "parser/stl/ParserSTL.h"
#include "parser/Parser.h"
class ConformalMesherLauncherTest : public ::testing::Test {

public:

    ConformalMesherLauncherTest() {
    }

    virtual ~ConformalMesherLauncherTest() {
    }

    void runUGRMesher(const string project, SmbData* smb) const {

        OptionsMesher optMsh;
        ConformalMesher *conf = NULL;
        const SmbData* smb;

        conf = ConformalMesherLauncher().run(optMsh, smb, NULL);

    }

    void compare(ProjectFile& cmshBase, ProjectFile& cmshNew) const{
        ifstream fileBase, fileNew;
        cmshBase.openAsInput(fileBase);
        cmshNew.openAsInput(fileNew);
        while(!fileBase.eof()&&!fileNew.eof()){
            string lineBase, lineNew;
            if(!fileBase.eof()){
                lineBase = next_line(fileBase);
            }
            if(!fileNew.eof()){
                lineNew = next_line(fileNew);
            }
            EXPECT_TRUE(lineBase==lineNew);
        }

        if(!fileBase.eof()){
            //todo expect: the remaining lines are comments
        }


        if(!fileBase.eof()){
            //todo expect the remaining lines are comments
        }
    }

private:
    string next_line (ifstream& file) const {
        string line;
        file>>line;
        string trimLine;
        trimLine = Parser::trim(line);
        while(!file.eof()&&trimLine.size()>0&&trimLine[0] == "*"){
            file>>line;
            string trimLine = Parser::trim(line);
        }
        return trimLine;
    }
};

#endif
