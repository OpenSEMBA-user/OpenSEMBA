#ifndef CONFORMALMESHERLAUNCHERTEST_H_
#define CONFORMALMESHERLAUNCHERTEST_H_

#include "gtest/gtest.h"
#include "parser/stl/ParserSTL.h"

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
    	//while(!file.eof()&&line == " comment symbol "){ //todo
		//	file>>line;
    	//}
		return line;
    }
};

#endif
