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
    void compare(ProjectFile& cmshBase, ProjectFile& cmshNew) const;

    void runConformalMesher(
            const string& project,
            UInt maxCellsPerLength,
            OptionsMesher* optsMesher,
            const string& subName) const;

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

    CVecR3 buildStep(const BoxR3& box, UInt maxCells) const {
        Real maxLength = box.getLength().getMax();
        Real step = maxLength / maxCells;
        return CVecR3(step, step, step);
    }

};

#endif
