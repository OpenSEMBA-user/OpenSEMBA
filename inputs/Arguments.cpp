/*
 * Arguments.cpp
 *
 *  Created on: Jul 19, 2013
 *      Author: luis
 */

#include "Arguments.h"

Arguments::Arguments() {
	argc = 0;
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
Arguments::printBasicInfo() const {
	cout<< " -- Arguments info ---" << endl;
	cout<< "Path: " << path << endl;
	cout<< "Filename: " << fileName << endl;
	cout<< "Project Folder: " << getProjectFolder() << endl;
	cout<< "Project Name: " << getProjectName() << endl;
}

char*
Arguments::getArgvpp(const unsigned int i, char* arg) {
	if (argc != (int) i) {
		return arg;
	} else {
		printHelp();
		cout << ">>>> A value must follow this last argument <<<<" << endl;
		abort(ARGUMENTS_ERROR);
	}
	return 0;
}

string
Arguments::getProjectFolder() const {
	char *cstr = new char[fileName.length() + 1];
	strcpy(cstr, fileName.c_str());
	string projectDir(dirname(cstr));
	projectDir += "/";
	delete [] cstr;
	return projectDir;
}

string
Arguments::getProjectName() const {
	char *cstr = new char[fileName.length() + 1];
	strcpy(cstr, fileName.c_str());
	string projectFile(basename(cstr));
	delete [] cstr;
	return removeExtension(projectFile);
}

string
Arguments::getFilename() const {
	return fileName;
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
		abort(ARGUMENTS_ERROR);
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
    	<< "  Authors: L. D. Angulo           ( lm@diazangulo.com ) " << endl
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
Arguments::printBasicHelp() const {
	cout<< " -n <folder>" << endl;
	cout<< "     Specifes a path to case folder." << endl;
	cout<< " -h, --help" << endl;
	cout<< "     Prints this help." << endl;
}

void
Arguments::printGoodbyeMessage(
 const string appName) const {
	cout << appName << " finished succesfully. :-)" << endl;
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

