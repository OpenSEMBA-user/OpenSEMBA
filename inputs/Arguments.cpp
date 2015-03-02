/*
 * Arguments.cpp
 *
 *  Created on: Jul 19, 2013
 *      Author: luis
 */

#include "Arguments.h"

Arguments::Arguments(const int argc,  const char* argv[]) {
    path_ = *argv++;
    if (argc == 1) {
        cout << " >>>> No arguments where given <<<< " << endl;
        printHelp();
        abort(EXIT_FAILURE);
    }
    for (int i = 1; i < argc; i++) {
        string str = *argv++;
        if (!str.compare("-n")) { // "-n" file name
            fileName_ = getArgvpp(++i, argc, *argv++);
        } else if (!str.compare("-h") || !str.compare("--help")) {
            printHelp();
            abort(EXIT_SUCCESS);
        }
    }
    if (!fExists(fileName_)) {
        printInfo();
        cerr<< "ERROR @ Arguments::getArguments(): "
            << "No input file was found. "
            << "These files existence were checked: "
            << fileName_ << endl;
        abort(EXIT_FAILURE);
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
	cout<< "Filename: " << fileName_ << endl;
	cout<< "Project Folder: " << getProjectFolder() << endl;
	cout<< "Project Name: " << getProjectName() << endl;
}

const char*
Arguments::getArgvpp(
        const unsigned int i,
        const int argc,
        const char* argv) {
	if (argc != (int) i) {
		return argv;
	} else {
		printHelp();
		cout << ">>>> A value must follow this last argument <<<<" << endl;
	}
	return 0;
}

string
Arguments::getProjectFolder() const {
	char *cstr = new char[fileName_.length() + 1];
	strcpy(cstr, fileName_.c_str());
	string projectDir(dirname(cstr));
	projectDir += "/";
	delete [] cstr;
	return projectDir;
}

string
Arguments::getProjectName() const {
	char *cstr = new char[fileName_.length() + 1];
	strcpy(cstr, fileName_.c_str());
	string projectFile(basename(cstr));
	delete [] cstr;
	return removeExtension(projectFile);
}

string
Arguments::getFilename() const {
	return fileName_;
}

void
Arguments::abort(int msg) const {
#ifdef USE_MPI
	MPI_Finalize();
#endif
	exit(msg);
}

string
Arguments::getFileNameFromProjectPath(
 const string str) const {
	string aux = str;
	unsigned int found = aux.find_last_of("/\\");
	while (found + 1 == aux.length() && aux.length() > 1) {
		aux = aux.substr(0, found);
		found = aux.find_last_of("/\\");
	}
	string folderName;
	if (found != string::npos) {
		folderName = aux.substr(found+1);
		unsigned int extension = folderName.find(".gid");
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

void
Arguments::printHelp() const {
	cout<< " -n <FILE>" << endl;
	cout<< "     Specifes a path to input file." << endl;
	cout<< " -h, --help" << endl;
	cout<< "     Prints this help." << endl;
}

void
Arguments::printGoodbyeMessage(
 const string appName) const {
	cout << appName << " finished succesfully. :-)" << endl;
}

string
Arguments::boolToStr(const bool param) const {
    if (param) {
        return "true";
    } else {
        return "false";
    }
}

string
Arguments::removeExtension(const string& fName) const {
    size_t pos = fName.rfind(".");
    if(pos == string::npos)  //No extension.
        return fName;
    if(pos == 0)    //. is at the front. Not an extension.
        return fName;
    return fName.substr(0, pos);
}

