// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
    Parser();
	virtual ~Parser();
	virtual void printInfo() const = 0;

    static inline string
    &trim(string &s) {
        return ltrim(rtrim(s));
    }

protected:
	ifstream f_in; // Input file stream.
	Real scalingFactor_;
    pair<CVecR3,CVecR3> boundaryPadding_, boundaryMeshSize_;

	static CVecR3 strToCartesianVector(const string& str);
	static bool strToBool(const string& value);
	static inline string &ltrim(string &s) {
		s.erase(s.begin(), find_if(s.begin(),
		 s.end(), not1(ptr_fun<int, int>(isspace))));
	 	return s;
	}
	static inline string &rtrim(string &s) {
		s.erase(find_if(s.rbegin(), s.rend(),
		 not1(ptr_fun<int, int>(isspace))).base(), s.end());
		return s;
	}
	static inline bool toBool(const UInt param) {
		assert(param == 0 || param == 1);
		if (param == 1) {
			return true;
		} else {
			return false;
		}
	}

	void postReadOperations(SmbData* res);
};
#endif
