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

#include <stdexcept>
#include <string>

#include "SwitchBase.h"

namespace SEMBA {
namespace Argument {

class MultiSwitch : public virtual SwitchBase {
public:
    MultiSwitch(const std::string&);
    MultiSwitch(const char&, const std::string&);
    MultiSwitch(const std::string&, const char&);
    MultiSwitch(const std::string&, const std::string&);
    MultiSwitch(const std::string&, const char&, const std::string&);
    MultiSwitch(const MultiSwitch&);
    virtual ~MultiSwitch();

    MultiSwitch* required  (const bool& = true);
    MultiSwitch* help      (const std::string&);
    MultiSwitch* defaultVal(const std::size_t&);

    void parse(Object&,
               std::vector<std::list<std::string>>&,
               std::vector<std::list<std::string>>&) const;

    void notParsed(Object&) const;

private:
    bool hasDefault_;
    std::size_t default_;

    void initDefault_();
};

} /* namespace Argument */
} /* namespace SEMBA */

