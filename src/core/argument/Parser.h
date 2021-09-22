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

#pragma once

#include "Formatter.h"
#include "Group.h"

namespace SEMBA {
namespace Argument {

class Parser : public Group {
public:
    Parser();
    virtual ~Parser();

    Parser& prog       (const std::string&);
    Parser& usage      (const std::string&);
    Parser& description(const std::string&);
    Parser& epilog     (const std::string&);
    Parser& prefixChars(const std::string&);
    Parser& allowAbbrev(const bool&);
    Parser& formatter  (Formatter*);
    Parser& args       (const int& argc, const char** argv);
    Parser& args       (const std::vector<std::string>&);

    const std::string& getProg  () const { return prog_;   }
    const std::string& getUsage () const { return usage_;  }
    const std::string& getEpilog() const { return epilog_; }

    Object parse();
    Object parse(const int& argc, const char** argv);
    Object parse(const std::vector<std::string>&);
    std::pair<Object, std::vector<std::string>> parseKnownArgs();
    std::pair<Object, std::vector<std::string>> parseKnownArgs(
            const int& argc, const char** argv);
    std::pair<Object, std::vector<std::string>> parseKnownArgs(
            const std::vector<std::string>&);

    void printUsage() const;
    void printHelp () const;

private:
    enum class ArgType {
        Positional,
        ShortOpt,
        LongOpt,
        Separator
    };

    //Input
    std::vector<std::string> input_;

    //Aux
    std::string prog_;
    std::string usage_;
    std::string epilog_;
    std::string prefixChars_;
    bool        allowAbbrev_;
    Formatter*  formatter_;

    void initDefault_();
    std::pair<Object, std::vector<std::string>> parse_();
    ArgType getArgType_(const std::string& arg) const;
    static void printErrorList_(const std::vector<std::list<std::string>>&);

private:
    //Erased
    Parser(const Parser&);
    Parser(Parser&&);

    Parser& operator=(const Parser&);
    Parser& operator=(Parser&&);
};

namespace Error {

class Empty : public Error {
public:
    Empty() : str_("No Arguments in parser") {}
    virtual ~Empty() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

class Ambiguous : public Error {
public:
    Ambiguous(const std::string& arg,
              const std::vector<std::string>& args) {
        std::stringstream aux;
        aux << "Ambiguous argument --" << arg
            << " that can match arguments";
        for (std::size_t i = 0; i < args.size(); i++) {
            if (i != 0) {
                aux << ",";
            }
            aux << " --" << args[i];
        }
        str_ = aux.str();
    }
    virtual ~Ambiguous() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

class Unknown : public Error {
public:
    Unknown(const std::vector<std::string>& args) {
        std::stringstream aux;
        aux << "Unrecognized arguments:";
        for (std::size_t i = 0; i < args.size(); i++) {
            aux << " " << args[i];
        }
        str_ = aux.str();
    }
    virtual ~Unknown() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

