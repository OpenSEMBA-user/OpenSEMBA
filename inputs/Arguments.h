/*
 * Arguments.h
 *
 *  Created on: Jul 19, 2013
 *      Author: luis
 */

#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>
#ifdef USE_MPI
#include <mpi.h>
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <libgen.h>
#include <cstring>

using namespace std;

class Arguments {
public:
   Arguments(const int argc, const char* argv[]);
   virtual ~Arguments();
   virtual string getProjectFolder() const;
   virtual string getProjectName() const;
   virtual string getFilename() const;
   virtual void printWelcomeMessage(
         const string appName,
         const string versionNumber) const;
   virtual void printGoodbyeMessage(
         const string appName) const;
protected:
   string path_;
   string fileName_;
   virtual void printInfo() const;
   virtual void printHelp() const;
   bool fExists(const string& filename) const;
   const char* getArgvpp(
           const unsigned int i,
           const int argc,
           const char *arg) const;
   string getFileNameFromProjectPath(
           const string projectPath) const;
   string boolToStr(const bool param) const;
   void abort(int msg) const;
private:
   string removeExtension(const string& fName) const;
};

#endif /* ARGUMENTS2_H_ */
