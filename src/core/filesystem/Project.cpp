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
#include <unistd.h>
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
    _chdir_(this->c_str())
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
    LPDWORD aux = NULL;
    if(GetBinaryTypeA(c_str(), aux) != 0) {
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
    char *cstr = new char[getFilename().length() + 1];
    std::strcpy(cstr, getFilename().c_str());
    PathFindFileName(cstr);
    std::string res(cstr);
#else
    std::string res(basename(const_cast<char*>(c_str())));
#endif
    return res;
}

std::string Project::getFolder() const {
    char *cstr = new char[length() + 1];
    strcpy(cstr, c_str());
#ifdef _WIN32
    PathRemoveFileSpec(cstr);
    std::string folder(cstr);
    if (!folder.empty()) {
        folder += "\\";
    }
#else
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

void Project::printInfo() const {
    std::cout << "Project file name: " << toStr() << std::endl;
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
    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
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

void Project::openFile(std::ofstream& file) const {
    openFile_(*this, file);
}

void Project::openFile_(const std::string& fileName,
                       std::ofstream& file) const {
    try {
        file.open(fileName.c_str());
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
        strOper.hwnd = NULL;
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

} /* namespace FileSystem */
} /* namespace SEMBA */

