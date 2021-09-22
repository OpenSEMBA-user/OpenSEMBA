

#pragma once

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

    std::string getFullPath() const;
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
    void setToCurrentWorkingDir();
    void openFile(std::ofstream& file,
                  const bool& = true) const;
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
    void openFile_(const std::string& fileName,
                   std::ofstream& file,
                   const bool& = true) const;
    std::string removeExtension_(const std::string& filename) const;
    void deleteDirIfExists_(const std::string& directory) const;
    bool checkExistance_(const std::string& fn) const;
    void initDir_(const std::string& fn) const;
};

} /* namespace FileSystem */
} /* namespace SEMBA */

