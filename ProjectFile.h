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

class ProjectFile {
public:
   ProjectFile();
   ProjectFile(const string& filename);
   string
   getFilename() const;
   string
   getProjectFolder() const;
   string
   getProjectName() const;
   bool
   canOpenFile() const;
   void
   printInfo() const;
   void
   setFilename(const string& filename);
   string
    getBasename() const;
protected:
   vector<string>
   getFilesBasenames(
         const string& directory,
         const string& extension) const;
   string
   removeExtension(
         const string& filename) const;
   void
   deleteDirIfExists(
         const string& directory) const;
   void
   openFile(
         const string& fileName,
         ofstream& file) const;
private:
   string filename_;
};
#endif /* SRC_COMMON_PROJECTFILE_H_ */
