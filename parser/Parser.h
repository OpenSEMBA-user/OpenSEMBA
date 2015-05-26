// File: parser.h
// =============== Parser procedures ==========================================
//  PURPOSE:
//  - Read raw data provided by the input file.
//  - Parser will not interpret the input, only has to ensure that all the data
//     is read. Information must remain in a similar way to the parsed data.
// ============================================================================
#ifndef PARSER_H_
#define PARSER_H_

#include <algorithm>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <libgen.h>
#include <sys/stat.h>

#include "SmbData.h"

class Parser {
public:
	virtual ~Parser();
	virtual void
	 printInfo() const = 0;

    static inline string
    &trim(string &s) {
        return ltrim(rtrim(s));
    }

protected:
	ifstream f_in; // Input file stream.
	CVecR3 strToCartesianVector(const string& str) const;
	bool strToBool(const string& value) const;
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
