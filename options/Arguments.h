/*
 * Arguments.h
 *
 *  Created on: Jul 19, 2013
 *      Author: luis
 */

#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <libgen.h>
#include <cstring>

using namespace std;

#include "Types.h"

class Arguments {
public:
    Arguments(const int argc, const char* argv[]);
    virtual ~Arguments();

    bool has(const string& arg) const;
    string get(const string& arg, const UInt i = 0) const;
    virtual string getProjectFolder() const;
    virtual string getProjectName() const;
    virtual string getFilename() const;
    UInt size() const;


    virtual void printWelcomeMessage(
            const string appName,
            const string versionNumber) const;
    virtual void printGoodbyeMessage(
            const string appName) const;
    void printInfo() const;
    void printHelp() const;

protected:
    string path_;
    map<string, vector<string>> args_;
    bool fExists(const string& filename) const;
    string getFileNameFromProjectPath(const string projectPath) const;
    string boolToStr(const bool param) const;
private:
    pair<string, vector<string>> readArgument(
            const int i, const int argc,  const char* argv[]) const;
    string removeExtension(const string& fName) const;
    string removeChars(const string& str, char* charsToRemove) const;
    bool isKey(string) const;
};

#endif /* ARGUMENTS2_H_ */
