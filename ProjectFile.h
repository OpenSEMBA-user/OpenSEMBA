/*
 * ProjectFile.h
 *
 *  Created on: Dec 19, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_PROJECTFILE_H_
#define SRC_COMMON_PROJECTFILE_H_

#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#include "Types.h"

class ProjectFile {
public:
    ProjectFile();
    ProjectFile(const string& filename);
    ProjectFile(const ProjectFile& rhs);
    virtual ~ProjectFile();

    ProjectFile& operator=(const ProjectFile& rhs);

    bool canOpenFile() const;
    bool canExecute() const;

    string getFilename() const;
    string getBasename() const;
    string getFolder() const;
    string getOutputFilename() const {
        return getFolder() + getOutputName();
    }
    string getOutputName() const {
        return getProjectName();
    }
    string getProjectName() const {
        return removeExtension(getBasename());
    }

    void setFilename(const string& filename);

    void printInfo() const;

protected:
    vector<string> getFilesBasenames(const string& directory,
                                     const string& extension) const;
    void openFile(ofstream& file) const;
    void openFile(const string& fileName, ofstream& file) const;
    string removeExtension(const string& filename) const;
    void deleteDirIfExists(const string& directory) const;

private:
    string filename_;
};



#endif /* SRC_COMMON_PROJECTFILE_H_ */
