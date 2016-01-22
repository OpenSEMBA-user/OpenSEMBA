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

#ifndef SEMBA_PARSER_PARSER_H_
#define SEMBA_PARSER_PARSER_H_

#include <algorithm>

#include "Data.h"

namespace SEMBA {
namespace Parser {

class Parser {
public:
    Parser();
    virtual ~Parser();
    virtual void printInfo() const = 0;

    static inline std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
    }

protected:
    std::ifstream f_in; // Input file stream.
    Math::Real scalingFactor_;
    std::pair<Math::CVecR3,Math::CVecR3> boundaryPadding_, boundaryMeshSize_;

    static Math::CVecR3 strToCartesianVector(const std::string& str);
    static bool strToBool(const std::string& value);

    static inline std::string& ltrim(std::string &s) {
        s.erase(s.begin(),
                std::find_if(s.begin(),
                             s.end(),
                             std::not1(std::ptr_fun<int, int>(isspace))));
        return s;
    }
    static inline std::string& rtrim(std::string &s) {
        s.erase(find_if(s.rbegin(),
                        s.rend(),
                        std::not1(
                            std::ptr_fun<int, int>(std::isspace))).base(),
                s.end());
        return s;
    }

    static inline bool toBool(const std::size_t param) {
        assert(param == 0 || param == 1);
        if (param == 1) {
            return true;
        } else {
            return false;
        }
    }

    void postReadOperations(Data* res);
};

} /* namespace Parser */
} /* namespace SEMBA */

#endif /* SEMBA_PARSER_PARSER_H_ */
