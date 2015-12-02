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

#ifndef SEMBA_ARGUMENT_VALUESINGLE_H_
#define SEMBA_ARGUMENT_VALUESINGLE_H_

#include "ValueBase.h"

namespace SEMBA {
namespace Argument {

class ValueSingle : public virtual ValueBase {
public:
    ValueSingle() {}
    virtual ~ValueSingle() {}

    virtual void parse(Object&,
                       std::vector<std::list<std::string>>&,
                       std::vector<std::list<std::string>>&) const;

    virtual void parseSingle(Object&,
                             std::list<std::string>&,
                             std::list<std::string>&) const = 0;
};

namespace Error {

class Multiple : public Error {
public:
    Multiple(const Argument& arg)
    :   arg_(arg),
        str_(std::string("Argument ") + arg.getArgName() +
             " declared multiple times") {}
    virtual ~Multiple() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const Argument& getArg() const { return arg_; }
private:
    const Argument& arg_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_VALUESINGLE_H_ */
