// File: parser.h
// =============== Parser procedures ==========================================
//  PURPOSE:
//  - Read raw data provided by the input file.
//  - Parser will not interpret the input, only has to ensure that all the data
//     is read. Information must remain in a similar way to the parsed data.
// ============================================================================
#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <complex>
#include <libgen.h>
#include <algorithm>
#include <sys/stat.h>
#include "../SmbData.h"

using namespace std;

class Parser {
public:
	virtual ~Parser();
	virtual void
	 printInfo() const = 0;
protected:
	ifstream f_in; // Input file stream.
	CVecR3
	 strToCartesianVector(const string& str) const;
	bool
	 strToBool(const string& value) const;
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
	static inline bool
	toBool(const UInt param) {
		assert(param == 0 || param == 1);
		if (param == 1) {
			return true;
		} else {
			return false;
		}
	}
};
#endif
