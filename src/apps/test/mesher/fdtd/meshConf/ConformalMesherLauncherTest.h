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

    void compare(const string &cmsh_base, const string &cmsh_new) const{

    	ifstream file_base, file_new;

        try {
        	file_base.open(cmsh_base);
        	file_new .open(cmsh_new);
        }
        catch(exception &e) {
        	// Todo Mess
        }
        file_base.seekg(0);
        file_new.seekg(0);
    	while(!file_base.eof()&&!file_new.eof()){
    		string line_base, line_new;
    		if(!file_base.eof()){
    			line_base = next_line(file_base);
    		}
    		if(!file_new.eof()){
    			line_new = next_line(file_new);
    		}
    		EXPECT_TRUE(line_base==line_new);
    	}

    	if(!file_base.eof()){
    		//todo
    		// Expect: the remaining lines are comments
    	}


    	if(!file_base.eof()){
    		//todo
    		// Expect the remaining lines are comments
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
