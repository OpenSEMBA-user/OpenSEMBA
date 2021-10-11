

#include "Project.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

#ifndef _WIN32
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#else
#include <direct.h>
#include <Shlwapi.h>
#endif

namespace SEMBA {
namespace FileSystem {

Project::Project() {

}

Project::Project(const std::string& filename) : std::string(filename) {
}

Project::Project(const Project& rhs) : std::string(rhs) {
}

Project::~Project() {

}

void Project::initDir_(const std::string& fn) const {
    std::string dirname = fn;
#ifdef _WIN32
    if (checkExistance_(dirname)) {
        return;
    }
    _mkdir(dirname.c_str());
#else
    if (checkExistance_(dirname)) {
        return;
    }
    if (mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
        throw std::logic_error("Folder could not be created.");
    }
#endif
}

void Project::makeDir() const {

    initDir_(*this);
}

void Project::changeDir() const {
#ifdef _WIN32
    _chdir(this->c_str());
#else
    chdir(this->c_str());
#endif
}

void Project::rmDir() const {
    std::string dir = *this;
    deleteDirIfExists_(dir);
}

bool Project::canOpen() const {
    std::ifstream file;
    file.open(c_str());
    bool res;
    if (file) {
        file.close();
        res = true;
    } else {
        res = false;
    }
    return res;
}

bool Project::canExecute() const {
#ifdef _WIN32
    DWORD aux;
    if(GetBinaryTypeA(c_str(), &aux) != 0) {
        return true;
    }
    return false;
#else
    struct stat st;
    if (stat(c_str(), &st) < 0) {
        return false;
    }
    if ((st.st_mode & S_IEXEC) != 0) {
        return true;
    }
    return false;
#endif
}

std::string Project::getFilename() const {
    return *this;
}

std::string Project::getBasename() const {
#ifdef _WIN32
    char* fname = new char[getFilename().size() + 1];
    char* ext   = new char[getFilename().size() + 1];
    _splitpath(getFilename().c_str(), nullptr, nullptr, fname, ext);
    std::string res = std::string(fname) + ext;
#else
    std::string res(basename(const_cast<char*>(c_str())));
#endif
    return res;
}

std::string Project::getFolder() const {
#ifdef _WIN32
    char* drive = new char[getFilename().size() + 1];
    char* dir = new char[getFilename().size() + 1];
    _splitpath(getFilename().c_str(), drive, dir, nullptr, nullptr);
    std::string folder = std::string(drive) + dir;
#else
    char *cstr = new char[length() + 1];
    strcpy(cstr, c_str());
    std::string folder(dirname(cstr));
    delete [] cstr;
#endif
    if (folder.find_last_of("/\\") != folder.length() - 1) {
#ifdef _WIN32
        folder += "\\";
#else
        folder += "/";
#endif
    }
    return folder;
}

void Project::setFilename(const std::string& filename) {
    std::string::operator=(filename);
}

std::string Project::toStr() const {
    return *this;
}

std::vector<std::string> Project::getFilesBasenames_(
        const std::string& directory,
        const std::string& extension) const {
    std::vector<std::string> files;
#ifdef _WIN32
    HANDLE hFind;
    WIN32_FIND_DATA data;

    hFind = FindFirstFile(directory.c_str(), &data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            files.push_back(std::string("data.cFileName"));
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
#else
    DIR *dir;
    struct dirent *ent;
    // Retrieves names of all files.
    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((ent = readdir (dir)) != nullptr) {
            files.push_back(ent->d_name);
        }
        closedir(dir);
    } else {
        std::cout << std::endl << "WARNING @ Project";
        std::cout << "Could not open directory to extract basenames. ";
        std::cout << "Tried: " << directory << std::endl;
    }
#endif
    // Stores files with names matching extension.
    std::vector<std::string> res;
    for (std::size_t i = 0; i < files.size(); i++) {
        size_t index = files[i].find(extension);
        if (index != std::string::npos) {
            res.push_back(files[i]);
        }
    }
    return res;
}

void Project::openFile(std::ofstream& file,
                       const bool& textMode) const {
    openFile_(*this, file, textMode);
}

void Project::openFile_(const std::string& fileName,
                        std::ofstream& file,
                        const bool& textMode) const {
    try {
        if (textMode) {
            file.open(fileName.c_str());
        } else {
            file.open(fileName.c_str(), std::ios::binary);
        }
    } catch(const std::exception&) {
        throw std::ios_base::failure(fileName + std::string(" not exists"));
    }
}

std::string Project::removeExtension_(const std::string& fName) const {
    size_t pos = fName.rfind(".");
    if (pos == std::string::npos) { //No extension.
        return fName;
    }
    if (pos == 0) {    //. is at the front. Not an extension.
        return fName;
    }
    return fName.substr(0, pos);
}

void Project::deleteDirIfExists_(const std::string& directory) const {
#ifdef _WIN32
    if (checkExistance_(directory)) {
        char *cstr = new char[directory.size() + 2];
        std::strcpy(cstr, directory.c_str());
        cstr[directory.size()  ] = 0;
        cstr[directory.size()+1] = 0;
        SHFILEOPSTRUCT strOper = { 0 };
        strOper.hwnd = nullptr;
        strOper.wFunc = FO_DELETE;
        strOper.pFrom = cstr;
        strOper.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
        if (SHFileOperation(&strOper) != 0) {
            std::cout << std::endl << "WARNING @ Project: ";
            std::cout << "Dir " << directory
                      << " deletion failed" << std::endl;
        }
        delete [] cstr;
    }
#else

    // Deletes if exists.
    if (checkExistance_(directory)) {
        std::string command = "rm -r ";
        command += directory;
        if (system(command.c_str())) {
            std::cout << std::endl << "WARNING @ Project: ";
            std::cout << "System command failed to execute " << command
                      << std::endl;
        }
    }
#endif
}

bool Project::checkExistance_(const std::string& fn) const {
    bool exists;
#ifdef _WIN32
    exists = false;;
    DWORD atrib = GetFileAttributesA(fn.c_str());
    if (atrib == INVALID_FILE_ATTRIBUTES) {
        exists = false;
    } else if (atrib & FILE_ATTRIBUTE_DIRECTORY) {
        exists = true;
    }
#else
    struct stat sb;
    exists = (stat(fn.c_str(), &sb) == 0);
    exists &= S_ISDIR(sb.st_mode);
#endif
    return exists;
}

Project Project::relativeTo(const Project& rhs) const {
    std::string rhsFolder;
    if (rhs.isFolder()) {
        rhsFolder = rhs.getFilename();
    } else {
        rhsFolder = rhs.getFolder();
    }
    std::string name = getFilename();
    std::string res = name.substr(name.find(rhsFolder) + rhsFolder.length(),
                                  name.length());
    return Project(res);
}

bool Project::isFolder() const {
#ifdef _WIN32
    DWORD atrib = GetFileAttributesA(c_str());
    if (atrib == INVALID_FILE_ATTRIBUTES) {
        return false;
    } else if (atrib & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }
    return false;
#else
    struct stat sb;
    stat(c_str(), &sb);
    return S_ISDIR(sb.st_mode);
#endif
}

void Project::openAsInput(std::ifstream& file) const {
    try {
        file.open(this->c_str());
    } catch(const std::exception&) {
        throw std::ios_base::failure(std::string("File can't be opened: ") +
                                     *this);
    }
}

void Project::exec(const std::string arguments) const {
    if (!canExecute()) {
        throw std::ios_base::failure("Can not execute " + *this);
    }
    std::string cmd = getFilename() + " " + arguments;
    system(cmd.c_str());
}

std::string Project::getExtension() const {
    size_t pos = rfind(".");
    if (pos == std::string::npos) {
        return *this;
    }
    if (pos == 0) {
        return *this;
    }
    return this->substr(pos, std::string::npos);
}

std::string Project::getFullPath() const {
    std::string res;
#ifndef _WIN32
    res = (const char*) realpath(this->getFilename().c_str(), nullptr);
    if (Project(res).isFolder()) {
        res += "/";
    }
#else
    TCHAR fullPath[MAX_PATH];
    GetFullPathName(this->c_str(), MAX_PATH, fullPath, nullptr);
    res = fullPath;
    if (Project(res).isFolder()) {
        res += "\\";
    }
#endif
    return res;
}

void Project::setToCurrentWorkingDir() {
#ifndef _WIN32
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::string cwdStr = cwd;
        *this = cwdStr;
    }
#else
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd))) {
        std::string cwdStr = cwd;
        *this = cwdStr;
    }
#endif
}



} /* namespace FileSystem */
} /* namespace SEMBA */
