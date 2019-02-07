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

#ifndef SEMBA_ARGUMENT_OPTIONBASE_H_
#define SEMBA_ARGUMENT_OPTIONBASE_H_

#include "Argument.h"

namespace SEMBA {
namespace Argument {

class OptionBase : public virtual Argument {
public:

    OptionBase();
    OptionBase(const char&);
    OptionBase(const std::string&);
    OptionBase(const char&, const std::string&);
    OptionBase(const OptionBase&);
    virtual ~OptionBase();

    std::string getArgName() const;

    bool        hasShortIdentifier() const { return hasShortId_; }
    const char& getShortIdentifier() const { return    shortId_; }

    bool               hasLongIdentifier() const { return hasLongId_; }
    const std::string& getLongIdentifier() const { return    longId_; }

private:
    bool     hasShortId_;
    char        shortId_;

    bool     hasLongId_;
    std::string longId_;

    void check_();
};

namespace Error {

class NoIdentifier : public Error {
public:
    NoIdentifier(const Argument& arg)
    :   arg_(arg),
        str_(std::string("Argumen: ") + arg.getName() +
             " has no valid identifier") {}
    virtual ~NoIdentifier() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const Argument& getArg() const { return arg_; }
private:
    const Argument& arg_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_OPTIONBASE_H_ */
