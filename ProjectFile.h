/*
 * ProjectFile.h
 *
 *  Created on: Dec 19, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_PROJECTFILE_H_
#define SRC_COMMON_PROJECTFILE_H_

#include <string>
#include <stdlib.h>
#include <stdio.h>
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
protected:
   string filename_;
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
   void openFile(
         const string& fileName, ofstream& file) const;
};

#endif /* SRC_COMMON_PROJECTFILE_H_ */
