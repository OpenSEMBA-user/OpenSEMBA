/*
 *        File: ParserNFDETools.h
 * -------------------------------------------------------------------------
 *      Author: damarro
 *  Created on: Mar 21, 2014
 * Description:
 *              
 *              
 */
#ifndef PARSERNFDETOOLS_H_
#define PARSERNFDETOOLS_H_

#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace {

bool isEmpty(const string &str) {
	if((str.find_first_of("0123456789")                 == string::npos) &&
	   (str.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos) &&
	   (str.find_first_of("abcdefghijklmnopqrstuvwxyz") == string::npos))
		return true;
	return false;
}

bool isComment(const string &str) {
	if(!str.empty())
		if(str[0] == '*')
			return true;
   
	return false;
}

bool isHeader(const string &str) {
	if(!str.empty())
		if(str[0] == '!')
			return true;
   
	return false;
}

void outputError(const string &section) {
   cout << "ERROR @ NFDE::readLine()" << endl;
   cerr << section << ": Exception reading file" << endl;
   exit(-1);
}

//	Complexx2
bool readLine(const string &section, const string &line, complex<double> &arg1,
                                                         complex<double> &arg2){
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Double
bool readLine(const string &section, const string &line, double &arg1) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
/*
//	Doublex2
bool readLine(const string &section, const string &line, double &arg1,
                                                         double &arg2) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
*/
//	Doublex3
bool readLine(const string &section, const string &line, double &arg1,
                                                         double &arg2,
                                                         double &arg3) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Doublex4
bool readLine(const string &section, const string &line, double &arg1,
                                                         double &arg2,
                                                         double &arg3,
                                                         double &arg4) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Doublex5
bool readLine(const string &section, const string &line, double &arg1,
                                                         double &arg2,
                                                         double &arg3,
                                                         double &arg4,
                                                         double &arg5) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	double long int
bool readLine(const string &section, const string &line, double &arg1,
                                                         long int &arg2) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
/*
//	Doublex3 string
bool readLine(const string &section, const string &line, double &arg1,
                                                         double &arg2,
                                                         double &arg3,
                                                         string &arg4,
                                                         bool upper = true) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
		if(upper)
			transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
*/
//	Doublex3 Stringx2
bool readLine(const string &section, const string &line, double &arg1,
                                                         double &arg2,
                                                         double &arg3,
                                                         string &arg4,
                                                         string &arg5) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
		transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
		transform(arg5.begin(), arg5.end(), arg5.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	int
bool readLine(const string &section, const string &line, int &arg1) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Intx2
bool readLine(const string &section, const string &line, int &arg1,
                                                         int &arg2) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Intx3
bool readLine(const string &section, const string &line, long int &arg1,
                                                         long int &arg2,
                                                         long int &arg3) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
/*
//	Intx4
bool readLine(const string &section, const string &line, int &arg1,
                                                         int &arg2,
                                                         int &arg3,
                                                         int &arg4) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
*/
//	long int Doublex2
/*
bool getline(ifstream &file, const string &section, long int &arg1, double &arg2, double &arg3) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}*/

//	Intx3 Doublex3
bool readLine(const string &section, const string &line, long int &arg1, long int &arg2, long int &arg3, double &arg4, double &arg5, double &arg6) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6;
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Intx3 string
bool readLine(const string &section, const string &line, long int &arg1, long int &arg2, long int &arg3, string &arg4) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
		transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
/*
//	Intx4 string
bool readLine(const string &section, const string &line, long int &arg1,
                                                         long int &arg2,
                                                         long int &arg3,
                                                         int &arg4,
                                                         string &arg5) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
		transform(arg5.begin(), arg5.end(), arg5.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
*/
//	Intx3 string double int
bool readLine(const string &section, const string &line, long int &arg1,
                                                         long int &arg2,
                                                         long int &arg3,
                                                         string &arg4,
                                                         double &arg5,
                                                         int &arg6) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6;
		transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Intx3 string int
bool readLine(const string &section, const string &line, long int &arg1,
                                                         long int &arg2,
                                                         long int &arg3,
                                                         string &arg4,
                                                         int &arg5) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
		transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Intx3 string Intx2
/*bool readLine(const string &section, const string &line, long int &arg1, long int &arg2, long int &arg3, string &arg4, long int &arg5, long int &arg6) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6;
		transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}*/

//	string
bool readLine(const string &section, const string &line, string &arg1, bool upper = true) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1;
		if(upper)
		    transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	Stringx2
bool readLine(const string &section, const string &line, string &arg1, string &arg2, bool upper = true) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2;
		transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
		if(upper)
		    transform(arg2.begin(), arg2.end(), arg2.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	string Doublex3
/*bool readLine(const string &section, const string &line, string &arg1, double &arg2, double &arg3, double &arg4) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
		transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}

//	string Doublex2 string
bool readLine(const string &section, const string &line, string &arg1, double &arg2, double &arg3, string &arg4) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
		transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
		transform(arg4.begin(), arg4.end(), arg4.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}*/

//	string Intx3
bool readLine(const string &section, const string &line, string &arg1, long int &arg2, long int &arg3, long int &arg4) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3 >> arg4;
		transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
/*
//	string long int string
bool readLine(const string &section, const string &line, string &arg1, long int &arg2, string &arg3) {
	stringstream buff;
	buff.exceptions(ifstream::failbit|ifstream::badbit);
	try {
		buff << line;
		buff >> arg1 >> arg2 >> arg3;
		transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
		transform(arg3.begin(), arg3.end(), arg3.begin(), ::toupper);
	}
	catch (const ifstream::failure &e) {
		outputError(section);
	}
	return true;
}
*/
}

#endif /* PARSERNFDETOOLS_H_ */
