/*
 * ProjectFile.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: luis
 */

#include "ProjectFile.h"

ProjectFile::ProjectFile() {

}

ProjectFile::ProjectFile(const string& filename) {
    filename_ = filename;
}

ProjectFile::ProjectFile(const ProjectFile& rhs) {
    filename_ = rhs.filename_;
}

ProjectFile::~ProjectFile() {

}

ProjectFile& ProjectFile::operator=(const ProjectFile& rhs) {
    if (this == &rhs) {
        return *this;
    }
    filename_ = rhs.filename_;
    return *this;
}

bool ProjectFile::canOpenFile() const {
    ifstream file;
    file.open(filename_.c_str());
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
    if (stat(filename_.c_str(), &st) < 0) {
        return false;
    }
    if ((st.st_mode & S_IEXEC) != 0) {
        return true;
    }
    return false;
}

string ProjectFile::getFilename() const {
    return filename_;
}

string ProjectFile::getBasename() const {
    string res(basename(const_cast<char*>(filename_.c_str())));
    return res;
}

string ProjectFile::getFolder() const {
    char *cstr = new char[filename_.length() + 1];
    strcpy(cstr, filename_.c_str());
    string projectDir(dirname(cstr));
#ifdef _WIN32
    projectDir += "\\";
#else
    projectDir += "/";
#endif
    delete [] cstr;
    return projectDir;
}

void ProjectFile::setFilename(const string& filename) {
    filename_ = filename;
}

void ProjectFile::printInfo() const {
    cout << "Project file name: " << filename_ << endl;
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
    openFile(filename_, file);
}

void ProjectFile::openFile(const string& fileName, ofstream& file) const {
    file.open(fileName.c_str());
    if (!file) {
        cerr << endl << "ERROR @ ProjectFile: ";
        cerr << "File for writing could not be opened. ";
        cerr << "File name: " << fileName << endl;
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
