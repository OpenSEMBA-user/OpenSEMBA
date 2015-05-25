/*
 * Arguments.cpp
 *
 *  Created on: Jul 19, 2013
 *      Author: luis
 */

#include "Arguments.h"

Arguments::ErrorArgumentNotExists::ErrorArgumentNotExists(const string& arg)
:   Error(string("Arguments: Argument ") + arg + string(" does not exist")) {

}

Arguments::ErrorArgumentNotExists::~ErrorArgumentNotExists() throw () {

}

Arguments::Arguments(const string& args) {
    stringstream iss(args);
    vector<string> arg;
    string str;
    while (getline(iss, str, ' ')) {
      arg.push_back(str);
    }
    build(arg);
}

Arguments::Arguments(const int argc,  const char* argv[]) {
    vector<string> arg(argv, argv + argc);
    build(arg);
}

void Arguments::build(const vector<string>& argv) {
    int argc = argv.size();
    for (Int i = 0; i < argc; i++) {
        const string str = argv[i];
        if (isKey(str)) {
            args_.insert(readArgument(i, argc, argv));
        }
    }
    if (argc == 1) {
        cout << " >>>> No arguments where given <<<< " << endl;
        printHelp();
    }
}

Arguments::~Arguments() {
	// TODO Auto-generated destructor stub
}

bool
Arguments::fExists(const string& filename) const {
   ifstream ifile(filename.c_str());
   return ifile;
}

string
Arguments::getProjectFolder() const {
	char *cstr = new char[getFilename().length() + 1];
	strcpy(cstr, getFilename().c_str());
	string projectDir(dirname(cstr));
#ifdef _WIN32
   projectDir += "\\";
#else
   projectDir += "/";
#endif
	delete [] cstr;
	return projectDir;
}

string
Arguments::getProjectName() const {
	char *cstr = new char[getFilename().length() + 1];
	strcpy(cstr, getFilename().c_str());
	string projectFile(basename(cstr));
	delete [] cstr;
	return removeExtension(projectFile);
}

string
Arguments::getFilename() const {
    return get("i");
}

string
Arguments::getFileNameFromProjectPath(
 const string str) const {
	string aux = str;
	UInt found = aux.find_last_of("/\\");
	while (UInt(found + 1) == aux.length() && aux.length() > 1) {
		aux = aux.substr(0, found);
		found = aux.find_last_of("/\\");
	}
	string folderName;
	if (found != string::npos) {
		folderName = aux.substr(found+1);
		UInt extension = folderName.find(".gid");
		if (extension != string::npos) {
			return folderName.substr(0, extension);
		} else {
			return folderName;
		}
	} else {
		cout << "Argument -p " << str << endl;
		cout << "Does not specify a folder." << endl;
	}
	return 0;
}

void
Arguments::printWelcomeMessage(
 const string appName,
 const string versionNumber) const {
	cout<< "========================================================" << endl
	    << "                -=- " << appName << " -=-               " << endl
	    << "========================================================" << endl
		<< " This is a proprietary software of:                     " << endl
		<< "             The University of Granada                  " << endl
		<< "                     www.ugr.es                         " << endl
		<< "  Authors: S. G. Garcia           ( salva@ugr.es )      " << endl
    	<< "           L. D. Angulo           ( lm@diazangulo.com ) " << endl
    	<< "           Miguel Ruiz-Cabello N. ( mcabello@ugr.es )   " << endl
    	<< "           Daniel Mateos          ( damarro@gmail.com ) " << endl
		<< "========================================================" << endl
		<< " - Version " << versionNumber
		<< " on " << __DATE__ << " " << __TIME__ << endl
#ifdef __GNUC__
		<< " - Compiled with: GNU C++ "	<< __GNUC__ << "." << __GNUC_MINOR__
		<< "." << __GNUC_PATCHLEVEL__ << endl
#endif
	 	<< "========================================================" << endl;
}

void Arguments::printHelp() const {
	cout<< " -i <FILE> [OPTIONS]" << endl;
	cout<< "     Specifes a path to input file." << endl;
}

void Arguments::printGoodbyeMessage(
 const string appName) const {
	cout << appName << " finished succesfully. :-)" << endl;
}

string Arguments::boolToStr(const bool param) const {
    if (param) {
        return "true";
    } else {
        return "false";
    }
}

string Arguments::removeExtension(const string& fName) const {
    size_t pos = fName.rfind(".");
    if(pos == string::npos)  //No extension.
        return fName;
    if(pos == 0)    //. is at the front. Not an extension.
        return fName;
    return fName.substr(0, pos);
}

string
Arguments::removeChars(const string& in, const char* charsToRemove) const {
    string res = in;
    for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
        res.erase(remove(res.begin(), res.end(), charsToRemove[i]), res.end());
    }
    return res;
}

bool Arguments::has(const string& arg) const {
    return (args_.find(arg) != args_.end());
}

string Arguments::get(const string& arg, const UInt i) const {
    if (!has(arg)) {
        throw ErrorArgumentNotExists(arg);
    }
    if (i > args_.find(arg)->second.size()) {
        throw ErrorArgumentNotExists(arg);
    }
    return args_.find(arg)->second[i];
}

UInt Arguments::size() const {
    return args_.size();
}

pair<string, vector<string>> Arguments::readArgument(
        const int pos,
        const int argc,
        const vector<string>& argv) const {
    string key;
    vector<string> value;
    string str = argv[pos];
    key = removeChars(str, "-");
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (pos == argc) {
        return pair<string, vector<string>> (key,value);
    }
    for (int i = pos+1; i < argc; i++) {
        string str = argv[i];
        string aux;
        transform(str.begin(), str.end(), aux.begin(), ::tolower);
        if (isKey(aux)) {
            return pair<string, vector<string>> (aux,value);
        }
        value.push_back(trim(str));
    }
    return pair<string, vector<string>> (key,value);
}

bool Arguments::isKey(const string str) const {
    return (str.find("-") == 0 || str.find("--") == 0);
}

bool Arguments::contains(const Arguments& rhs) const {
    map<string,vector<string>>::const_iterator itRHS, it;
    for (itRHS = rhs.args_.begin(); itRHS != rhs.args_.end(); ++itRHS) {
        it = args_.find(itRHS->first);
        if (it == args_.end()) {
            return false;
        }
        if (it->second.size() != itRHS->second.size()) {
            return false;
        }
        for (UInt i = 0; i < it->second.size(); i++) {
            if (it->second[i] != itRHS->second[i]) {
                return false;
            }
        }
    }
    return true;
}

string Arguments::toStr() const {
    stringstream ss;
    map<string,vector<string>>::const_iterator it;
    for (it = args_.begin(); it != args_.end(); ++it) {
        ss << " -" << it->first;
        for (UInt i = 0; i < it->second.size(); i++) {
            ss  << " " << it->second[i];
        }
    }
    return ss.str();
}

void
Arguments::printInfo() const {
    cout<< " -- Arguments info ---" << endl;
    cout<< "Filename: " << getFilename() << endl;
    cout<< "Project Folder: " << getProjectFolder() << endl;
    cout<< "Project Name: " << getProjectName() << endl;
    cout<< "Arguments read: " << size() << endl;
    map<string,vector<string>>::const_iterator it;
    cout << " - Key --- Values -" << endl;
    for (it = args_.begin(); it != args_.end(); ++it) {
        cout << it->first << ": ";
        for (UInt i = 0; i < it->second.size(); i++) {
            cout << it->second[i] << " ";
        }
        cout << endl;
    }
}

