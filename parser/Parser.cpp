// File: parser.cpp
// =============== Includes headers ===========================================
#ifndef PARSER_H_
#include "Parser.h"
#endif

Parser::~Parser() {

}

CVecR3
Parser::strToCartesianVector(
 const string& str) const {
	istringstream iss(str);
	string sub;
	CartesianVector<Real,3> res;
	for (UInt i = 0; i < 3; i++) {
		iss >> sub;
		res(i) = atof(sub.c_str());
	}
	return res;
}

bool
Parser::strToBool(const string& value) const {
	if (atoi(value.c_str()) == 1) {
		return true;
	} else {
		return false;
	}
}

