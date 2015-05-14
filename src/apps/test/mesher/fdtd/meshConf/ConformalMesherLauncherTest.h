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

    void runUGRMesher_old(const ProjectFile& ugrMesherUbication,
                          const string& args , SmbData* smb) const {
        OptionsMesher optMsh;
        string runString;
        runString = ugrMesherUbication +"ugrMesher" + args;
        exec(runString.c_str());
    }

    void runUGRMesher_new(const string project, SmbData* smb) const {
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
            next_line (fileBase);
            EXPECT_TRUE(fileBase.eof());
        }


        if(!fileBase.eof()){
            next_line (fileNew);
            EXPECT_TRUE(fileNew.eof());
        }
    }

private:
    string next_line (ifstream& file) const {
        if(!file.eof()){
            string line;
            file>>line;
            while(!file.eof()&&isCommentLine (line)){
                file>>line;
            }
            return line;
        }
        return "";
    }

    static bool isCommentLine (string& line){
        string trimLine = Parser::trim(line);
        return ( trimLine.empty()   ||
                (trimLine.size()==0)||
                (trimLine.substr(0,0) ==string("*") ));
    }


    static std::string exec(const char* cmd) {
        FILE* pipe = popen(cmd, "r");
        if (!pipe) return "ERROR";
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        return result;
    }

};

#endif
