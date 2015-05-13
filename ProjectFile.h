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

class ProjectFile : public string {
public:
    ProjectFile();
    ProjectFile(const string& filename);
    ProjectFile(const ProjectFile& rhs);
    virtual ~ProjectFile();

    bool canOpen() const;
    bool canExecute() const;
    bool isFolder() const;

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
    ProjectFile relativeTo(const ProjectFile& rhs) const;

    void setFilename(const string& filename);
    void openFile(ofstream& file) const;
    void openAsInput(ifstream& file) const;

    void printInfo() const;
    string toStr() const;

    std::ostream& operator<<(ostream& os) {
        return os << toStr();
    }

protected:
    vector<string> getFilesBasenames(const string& directory,
                                     const string& extension) const;
    void openFile(const string& fileName, ofstream& file) const;
    string removeExtension(const string& filename) const;
    void deleteDirIfExists(const string& directory) const;
    void initDir_(const string& fn);
};


#endif /* SRC_COMMON_PROJECTFILE_H_ */
