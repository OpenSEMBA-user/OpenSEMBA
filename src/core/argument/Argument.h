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

#ifndef SEMBA_ARGUMENT_ARGUMENT_H_
#define SEMBA_ARGUMENT_ARGUMENT_H_

#include <string>

#include "Object.h"

namespace SEMBA {
namespace Argument {

class Argument {
public:
    Argument();
    Argument(const std::string&);
    Argument(const Argument&);
    virtual ~Argument();

    const  std::string& getName()    const { return name_; }
    virtual std::string getArgName() const { return name_; }

    const bool&        getRequired() const { return required_; }
    const std::string& getHelp() const { return help_; }
    const std::list<std::string>& getMetavar() const { return metavar_; }
    const std::list<std::string>& getDefault() const { return default_; }
    const std::vector<std::list<std::string>>& getChoices() const {
        return choices_;
    }

    virtual Argument& required  (const bool& = true);
    virtual Argument& help(const std::string&);
    virtual Argument& metavar(const std::list<std::string>&);
    virtual Argument& defaultVal(const std::list<std::string>&);
    virtual Argument& choices(const std::vector<std::list<std::string>>&);

    virtual void parse(Object&,
                       std::vector<std::list<std::string>>&,
                       std::vector<std::list<std::string>>&) const = 0;

    virtual void noParsed(Object&) const;

private:
    std::string name_;

    bool required_;
    std::string help_;
    std::list<std::string> metavar_;
    std::list<std::string> default_;
    std::vector<std::list<std::string>> choices_;

    void initDefault_();
};

namespace Error {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

class Required : public Error {
public:
    Required(const SEMBA::Argument::Argument& arg)
    :   arg_(arg),
        str_(std::string("Argument ") + arg.getArgName() +
             " is required") {}
    virtual ~Required() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const SEMBA::Argument::Argument& getArg() const { return arg_; }
private:
    const SEMBA::Argument::Argument& arg_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_ARGUMENT_H_ */
