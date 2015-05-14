/*
 * ProjectFile.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: luis
 */

#include "ProjectFile.h"

ProjectFile::ProjectFile() {

}

ProjectFile::ProjectFile(const string& filename) : string(filename) {
}

ProjectFile::ProjectFile(const ProjectFile& rhs) : string(rhs) {
}

ProjectFile::~ProjectFile() {

}

void ProjectFile::initDir_(const string& fn) {
    string dirname = fn + ".vtk";
#ifdef _WIN32
    mkdir(dirname.c_str());
#else
    mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

bool ProjectFile::canOpen() const {
    ifstream file;
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

bool ProjectFile::canExecute() const {
    struct stat st;
    if (stat(c_str(), &st) < 0) {
        return false;
    }
    if ((st.st_mode & S_IEXEC) != 0) {
        return true;
    }
    return false;
}

string ProjectFile::getFilename() const {
    return *this;
}

string ProjectFile::getBasename() const {
    string res(basename(const_cast<char*>(c_str())));
    return res;
}

string ProjectFile::getFolder() const {
    char *cstr = new char[length() + 1];
    strcpy(cstr, c_str());
    string folder(dirname(cstr));
    if (folder.find_last_of("/") != folder.length() - 1) {
        folder += "/";
    }
    delete [] cstr;
    return folder;
}

void ProjectFile::setFilename(const string& filename) {
    string::operator=(filename);
}

string ProjectFile::toStr() const {
    return *this;
}

void ProjectFile::printInfo() const {
    cout << "Project file name: " << toStr() << endl;
}

vector<string> ProjectFile::getFilesBasenames(const string& directory,
        const string& extension) const {
    DIR *dir;
    struct dirent *ent;
    vector<string> files;
    // Retrieves names of all files.
    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            files.push_back(ent->d_name);
        }
        closedir(dir);
    } else {
        cerr << endl << "ERROR @ ProjectFile";
        cerr << "Could not open directory to extract basenames. ";
        cerr << "Tried: " << directory << endl;
    }
    // Stores files with names matching extension.
    vector<string> res;
    for (UInt i = 0; i < files.size(); i++) {
        size_t index = files[i].find(extension);
        if (index != string::npos) {
            res.push_back(files[i]);
        }
    }
    return res;
}

void ProjectFile::openFile(ofstream& file) const {
    openFile(*this, file);
}

void ProjectFile::openFile(const string& fileName, ofstream& file) const {
    try {
        file.open(fileName.c_str());
    }
    catch(exception &e) {
        cerr << endl << "ERROR @ ProjectFile: "
                << "File can't be opened: " << fileName << endl;
    }
}

string ProjectFile::removeExtension(const string& fName) const {
    size_t pos = fName.rfind(".");
    if (pos == string::npos) { //No extension.
        return fName;
    }
    if (pos == 0) {    //. is at the front. Not an extension.
        return fName;
    }
    return fName.substr(0, pos);
}

void ProjectFile::deleteDirIfExists(const string& directory) const {
    // Checks existence
    struct stat sb;
    bool exists = (stat(directory.c_str(), &sb) == 0);
    exists &= S_ISDIR(sb.st_mode);
    // Deletes if exists.
    if (exists) {
#ifdef _WIN32
        string command = "rmdir /s /q ";
#else
        string command = "rm -r ";
#endif
        command += directory;
        if (system(command.c_str())) {
            cerr << endl << "ERROR @ ProjectFile: ";
            cerr << "System command failed to execute " << command << endl;
        }
    }
}

ProjectFile ProjectFile::relativeTo(const ProjectFile& rhs) const {
    string rhsFolder;
    if (rhs.isFolder()) {
        rhsFolder = rhs.getFilename();
    } else {
        rhsFolder = rhs.getFolder();
    }
    string name = getFilename();
    string res = name.substr(name.find(rhsFolder) + rhsFolder.length(), name.length());
    return ProjectFile(res);
}

bool ProjectFile::isFolder() const {
    struct stat sb;
    stat(c_str(), &sb);
    return S_ISDIR(sb.st_mode);
}

void ProjectFile::openAsInput(ifstream& file) const {
    try {
        file.open(this->c_str());
    }
    catch(exception &e) {
        cerr << endl << "ERROR @ ProjectFile: "
                << "File can't be opened: " << *this << endl;
    }
}

void ProjectFile::exec(const string arguments) const {
    if (!canExecute()) {
        cerr << endl << "ERROR @ ProjectFile:"
                << "Can not execute" << endl;
    }
#ifndef _WIN32
    string cmd = getFilename() + " " + arguments;
    system(cmd.c_str());
#else
    cerr << endl << "ERROR @ ProjectFile: "
            << "Not implemented fo windows." << endl;
    // TODO Dani: Implementar en windows.
#endif
}
