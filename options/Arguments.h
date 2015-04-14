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

#include "Types.h"

class Arguments {
public:
    Arguments();
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

    bool have(const string& arg) const;
    string get(const string& arg, const UInt i = 0) const;

    void printInfo() const;
    void printHelp() const;

protected:
    string path_;
    string fileName_;
    bool fExists(const string& filename) const;
    const char* getArgvpp(
            const UInt i,
            const Int argc,
            const char *arg) const;
    string getFileNameFromProjectPath(
            const string projectPath) const;
    string boolToStr(const bool param) const;
    void abort(Int msg) const;
private:
    string removeExtension(const string& fName) const;
};

#endif /* ARGUMENTS2_H_ */
