
#pragma once

#include <algorithm>

#include "Data.h"

namespace SEMBA {
namespace Parsers {

class Parser {
public:
    Parser(const std::string& filenameIn) : 
        filename(filenameIn) {};
    
    virtual Data read() const = 0;

protected:
    FileSystem::Project filename;

    static inline std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
    }

    static Math::CVecR3 strToCartesianVector(const std::string& str);
    static bool strToBool(const std::string& value);

    static inline std::string& ltrim(std::string &s) {
        s.erase(s.begin(),
                std::find_if(s.begin(), s.end(),
                             [](int c) {return !std::isspace(c); })
        );
        return s;
    }
    static inline std::string& rtrim(std::string &s) {
        s.erase(find_if(s.rbegin(), s.rend(),
                        [](int c) {return !std::isspace(c); }).base(),
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

    void postReadOperations(Data& res) const;
};

} /* namespace Parser */
} /* namespace SEMBA */
