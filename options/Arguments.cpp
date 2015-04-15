/*
 * Arguments.cpp
 *
 *  Created on: Jul 19, 2013
 *      Author: luis
 */

#include "Arguments.h"

Arguments::Arguments(const int argc,  const char* argv[]) {
    path_ = argv[0];
    for (Int i = 1; i < argc; i++) {
        const string str = argv[i];
        if (isKey(str)) {
            args_.insert(readArgument(i, argc, argv));
        }
    }
    if (argc == 1) {
        cout << " >>>> No arguments where given <<<< " << endl;
        printHelp();
    }
    if (!fExists(getFilename())) {
        printInfo();
        cerr << endl << "ERROR @ Arguments::getArguments(): "
            << "No input file was found. "
            << "These files existence were checked: "
            << getFilename() << endl;
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

void
Arguments::printInfo() const {
	cout<< " -- Arguments info ---" << endl;
	cout<< "Path: " << path_ << endl;
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
    if (has("i")) {
        return get("i");
    }
    cerr << endl << "ERROR @ Arguments: No filename read." << endl;
    return string();
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
Arguments::removeChars(const string& in, char* charsToRemove) const {
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
        cerr << endl << "ERROR @ Arguments: "
                << "Argument " << arg << " does not exist." << endl;
    }
    return args_.find(arg)->second[i];
}

UInt Arguments::size() const {
    return args_.size();
}

pair<string, vector<string>> Arguments::readArgument(
        const int pos,
        const int argc,
        const char* argv[]) const {
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
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        if (isKey(str)) {
            break;
        }
        value.push_back(str);
    }
    return pair<string, vector<string>> (key,value);
}

bool Arguments::isKey(const string str) const {
    return (str.find("-") == 0 || str.find("--") == 0);
}
