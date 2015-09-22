// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
#include "base/error/ErrorFile.h"

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
    string getExtension() const;
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

    void exec(const string arguments = string()) const;

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
