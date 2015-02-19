/*
 * ProjectFile.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: luis
 */

#include "ProjectFile.h"

ProjectFile::ProjectFile() {

}

string ProjectFile::getBasename() const {
   string res(basename(const_cast<char*>(filename_.c_str())));
   return res;
}


ProjectFile::ProjectFile(const string& filename) {
   filename_ = filename;
}

void
ProjectFile::setFilename(const string& filename) {
   filename_ = filename;
}

void ProjectFile::deleteDirIfExists(const string& directory) const {
   // Checks existence
   struct stat sb;
   bool exists = (stat(directory.c_str(), &sb) == 0);
   exists &= S_ISDIR(sb.st_mode);
   // Deletes if exists.
   if (exists) {
      string command = "rm -r ";
      command += directory;
      if (system(command.c_str())) {
          cerr << "ERROR @ ProjectFile: "
           << "System command failed to execute " << command << endl;
      }
   }
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

void ProjectFile::openFile(ofstream& file) const {
   openFile(filename_, file);
}

void ProjectFile::openFile(
      const string& fileName,
      ofstream& file) const {
   file.open(fileName.c_str());
   if (!file) {
      cerr << "ERROR @ ProjectFile: ";
      cerr << "File for writing could not be opened. ";
      cerr << "File name: " << fileName << endl;
   }
}

vector<string>
ProjectFile::getFilesBasenames(
      const string& directory,
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
      cerr << "ERROR @ ProjectFile";
      cerr << "Could not open directory to extract basenames.";
      cerr << "Tried: " << directory << endl;
   }
   // Stores files with names matching extension.
   vector<string> res;
   for (uint i = 0; i < files.size(); i++) {
      size_t index = files[i].find(extension);
      if (string::npos != index) {
         res.push_back(files[i]);
      }
   }
   return res;
}

string
ProjectFile::removeExtension(
      const string& fName) const {
   size_t pos = fName.rfind(".");
   if (pos == string::npos) { //No extension.
      return fName;
   }
   if (pos == 0) {    //. is at the front. Not an extension.
      return fName;
   }
   return fName.substr(0, pos);
}

string
ProjectFile::getFilename() const {
   return filename_;
}

string
ProjectFile::getProjectFolder() const {
   char *cstr = new char[filename_.length() + 1];
   strcpy(cstr, filename_.c_str());
   string projectDir(dirname(cstr));
   projectDir += "/";
   delete [] cstr;
   return projectDir;
}

void ProjectFile::printInfo() const {
   cout << "Project file name: " << filename_ << endl;
}

