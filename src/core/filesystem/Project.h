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

#ifndef SEMBA_FILESYSTEM_PROJECT_H_
#define SEMBA_FILESYSTEM_PROJECT_H_

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#include <fstream>
#include <string>
#include <vector>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <direct.h>
#endif

namespace SEMBA {
namespace FileSystem {

class Project : public std::string {
public:
    Project();
    Project(const std::string& filename);
    Project(const Project& rhs);
    virtual ~Project();

    bool canOpen() const;
    bool canExecute() const;
    bool isFolder() const;

    std::string getFilename() const;
    std::string getBasename() const;
    std::string getFolder() const;
    std::string getExtension() const;
    std::string getOutputFilename() const {
        return getFolder() + getOutputName();
    }
    std::string getOutputName() const {
        return getProjectName();
    }
    std::string getProjectName() const {
        return removeExtension_(getBasename());
    }
    Project relativeTo(const Project& rhs) const;

    void setFilename(const std::string& filename);
    void openFile(std::ofstream& file) const;
    void openAsInput(std::ifstream& file) const;

    void exec(const std::string arguments = std::string()) const;

    void printInfo() const;
    std::string toStr() const;

    std::ostream& operator<<(std::ostream& os) {
        return os << toStr();
    }

    void makeDir() const;
    void changeDir() const;
    void rmDir() const;

protected:
    std::vector<std::string> getFilesBasenames_(
            const std::string& directory,
            const std::string& extension) const;
    void openFile_(const std::string& fileName, std::ofstream& file) const;
    std::string removeExtension_(const std::string& filename) const;
    void deleteDirIfExists_(const std::string& directory) const;
    void initDir_(const std::string& fn) const;
};

} /* namespace FileSystem */
} /* namespace SEMBA */

#endif /* SEMBA_FILESYSTEM_PROJECT_H_ */
