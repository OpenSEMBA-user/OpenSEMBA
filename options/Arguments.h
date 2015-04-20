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
#include <sstream>
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
    Arguments(const string& arg);
    Arguments(const int argc, const char* argv[]);
    virtual ~Arguments();

    bool has(const string& arg) const;
    string get(const string& arg, const UInt i = 0) const;
    virtual string getProjectFolder() const;
    virtual string getProjectName() const;
    virtual string getFilename() const;
    UInt size() const;

    bool contains(const Arguments& rhs) const;

    virtual void printWelcomeMessage(
            const string appName,
            const string versionNumber) const;
    virtual void printGoodbyeMessage(
            const string appName) const;

    string toStr() const;
    void printInfo() const;
    void printHelp() const;

    friend std::ostream& operator<<(ostream& os, const Arguments& lay) {
       return os << lay.toStr();
    }


protected:
    map<string, vector<string>> args_;
    bool fExists(const string& filename) const;
    string getFileNameFromProjectPath(const string projectPath) const;
    string boolToStr(const bool param) const;
private:
    pair<string, vector<string>> readArgument(
            const int i, const int argc,  const vector<string>& argv) const;
    string removeExtension(const string& fName) const;
    string removeChars(const string& str, const char* charsToRemove) const;
    bool isKey(string) const;
    void build(const vector<string>& args);

    static inline string
     &ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(),
         s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
    }
    static inline string
     &rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(),
         not1(ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
    }
    static inline string
    &trim(string &s) {
        return ltrim(rtrim(s));
    }
};

#endif /* ARGUMENTS2_H_ */
