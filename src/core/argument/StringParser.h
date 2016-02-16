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

#ifndef SEMBA_ARGUMENT_STRINGPARSER_H_
#define SEMBA_ARGUMENT_STRINGPARSER_H_

#include <algorithm>
#include <cctype>
#include <exception>
#include <functional>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace SEMBA {
namespace Argument {
namespace StringParser {

namespace Error {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

class NotEnough : public Error {
public:
    NotEnough() : str_("Not enough values") {}
    virtual ~NotEnough() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

class TooMany : public Error {
public:
    TooMany() : str_("Too many values") {}
    virtual ~TooMany() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

class Incorrect : public Error {
public:
    Incorrect() : str_("Invalid value") {}
    virtual ~Incorrect() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Error */

template<typename T>
struct Parser {
    static bool parse(T& out, std::stringstream& input) {
        input >> out;
        return !input.fail();
    }
};

template<>
struct Parser<std::string> {
    static bool parse(std::string& out, std::stringstream& input) {
        out = input.str();
        input.clear();
        input.str(std::string());
        return !out.empty();
    }
};

struct Details {
    static inline std::string& ltrim(std::string& s) {
        s.erase(s.begin(),
                std::find_if(s.begin(),
                             s.end(),
                             std::not1(
                             std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    static inline std::string& rtrim(std::string& s) {
        s.erase(
        std::find_if(s.rbegin(),
                     s.rend(),
                     std::not1(
                     std::ptr_fun<int, int>(std::isspace))).base(),
                     s.end());
        return s;
    }

    static inline std::string& trim(std::string& s) {
        return ltrim(rtrim(s));
    }

    template<typename T>
    struct ParserHelper {
        static void parse(T& out,
                          std::list<std::string>& outlist,
                          std::list<std::string>& inlist,
                          std::size_t p) {
            if (inlist.empty()) {
                throw Error::NotEnough();
            }
            bool success = false;
            std::string aux1;
            while (!success) {
                if (inlist.empty()) {
                    outlist.push_back(aux1);
                    throw Error::Incorrect();
                }
                aux1 += inlist.front();
                inlist.pop_front();

                std::stringstream aux2(aux1);
                if(StringParser::Parser<T>::parse(out, aux2)) {
                    std::string used, remain;
                    used = aux1;
                    std::getline(aux2, remain);
                    trim(used);
                    trim(remain);
                    if(!remain.empty()) {
                        used = used.substr(0, used.find(remain));
                        inlist.push_front(remain);
                        trim(used);
                    }
                    if(!used.empty()) {
                        outlist.push_back(used);
                    }
                    success = true;
                } else {
                    aux1 += " ";
                }
            }
        }
    };

    template<typename T>
    struct ParserHelper<std::vector<T>> {
        static void parse(std::vector<T>& out,
                          std::list<std::string>& outlist,
                          std::list<std::string>& inlist,
                          std::size_t p) {
            std::list<std::string> rem;
            std::string aux1;
            while (!inlist.empty()) {
                T outElem;
                aux1 += inlist.front();
                rem.push_back(inlist.front());
                inlist.pop_front();
                std::stringstream aux2(aux1);
                if(StringParser::Parser<T>::parse(outElem, aux2)) {
                    out.push_back(outElem);
                    std::string used, remain;
                    used = aux1;
                    std::getline(aux2, remain);
                    trim(used);
                    trim(remain);
                    rem.clear();
                    if(!remain.empty()) {
                        used = used.substr(0, used.find(remain));
                        rem.push_front(remain);
                        trim(used);
                    }
                    if(!used.empty()) {
                        outlist.push_back(used);
                    }
                    aux1 = remain;
                } else {
                    aux1 += " ";
                }
            }
            inlist = rem;
        }
    };

    template<std::size_t I, typename... T>
    struct Parser {
        static void parse(std::tuple<T...>& tuple,
                          std::list<std::string>& outlist,
                          std::list<std::string>& inlist) {
            const std::size_t P = std::tuple_size<std::tuple<T...>>::value-I-1;
            typedef typename std::tuple_element<P, std::tuple<T...>>::type
                Type;
            ParserHelper<Type>::parse(std::get<P>(tuple), outlist, inlist, P);
            Parser<I-1, T...>::parse(tuple, outlist, inlist);
        }
    };

    template<typename... T>
    struct Parser<0, T...> {
        static void parse(std::tuple<T...>& tuple,
                          std::list<std::string>& outlist,
                          std::list<std::string>& inlist) {
            const std::size_t P = std::tuple_size<std::tuple<T...>>::value - 1;
            typedef typename std::tuple_element<P, std::tuple<T...>>::type
                Type;
            ParserHelper<Type>::parse(std::get<P>(tuple), outlist, inlist, P);
        }
    };
};

template<typename T>
T parse(const std::list<std::string>& input) {
    std::tuple<T> res;
    std::list<std::string> auxin;
    std::list<std::string> auxout(input.begin(), input.end());
    Details::Parser<0, T>::parse(res, auxin, auxout);
    if (!auxout.empty()) {
        throw Error::TooMany();
    }
    return std::get<0>(res);
}

template<typename T1, typename T2>
std::pair<T1, T2> parse(const std::list<std::string>& input) {
    std::tuple<T1, T2> res;
    std::list<std::string> auxin;
    std::list<std::string> auxout(input.begin(), input.end());
    Details::Parser<1, T1, T2>::parse(res, auxin, auxout);
    if (!auxout.empty()) {
        throw Error::TooMany();
    }
    return std::make_pair(std::get<0>(res), std::get<1>(res));
}

template<typename T1, typename T2, typename T3, typename... T>
std::tuple<T1, T2, T3, T...> parse(const std::list<std::string>& input) {
    std::tuple<T1, T2, T3, T...> res;
    std::list<std::string> auxin;
    std::list<std::string> auxout(input.begin(), input.end());
    Details::Parser<sizeof...(T)+2, T1, T2, T3, T...>::parse(res,
                                                             auxin, auxout);
    if (!auxout.empty()) {
        throw Error::TooMany();
    }
    return res;
}

template<typename T>
T parse(const std::string& input) {
    return parse<T>({input});
}

template<typename T1, typename T2>
std::pair<T1, T2> parse(const std::string& input) {
    return parse<T1, T2>({input});
}

template<typename T1, typename T2, typename T3, typename... T>
std::tuple<T1, T2, T3, T...> parse(const std::string& input) {
    return parse<T1, T2, T3, T...>({input});
}

template<typename T>
T parse(std::list<std::string>& input, std::list<std::string>& output) {
    std::tuple<T> res;
    output.clear();
    std::swap(input, output);
    Details::Parser<0, T>::parse(res, input, output);
    return std::get<0>(res);
}

template<typename T1, typename T2>
std::pair<T1, T2> parse(std::list<std::string>& input,
                        std::list<std::string>& output) {
    std::tuple<T1, T2> res;
    output.clear();
    std::swap(input, output);
    Details::Parser<1, T1, T2>::parse(res, input, output);
    return std::make_pair(std::get<0>(res), std::get<1>(res));
}

template<typename T1, typename T2, typename T3, typename... T>
std::tuple<T1, T2, T3, T...> parse(std::list<std::string>& input,
                                   std::list<std::string>& output) {
    std::tuple<T1, T2, T3, T...> res;
    output.clear();
    std::swap(input, output);
    Details::Parser<sizeof...(T)+2, T1, T2, T3, T...>::parse(res,
                                                             input, output);
    return res;
}

} /* namespace StringParser */
} /* namespace Objects */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_STRINGPARSER_H_ */
