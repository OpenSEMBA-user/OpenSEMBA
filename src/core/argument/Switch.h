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

#ifndef SEMBA_ARGUMENT_SWITCH_H_
#define SEMBA_ARGUMENT_SWITCH_H_

#include <stdexcept>
#include <string>

#include "ValueSingle.h"
#include "SwitchBase.h"

namespace SEMBA {
namespace Argument {

class Switch : public SwitchBase {
public:
    Switch(const std::string&);
    Switch(const char&, const std::string&);
    Switch(const std::string&, const char&);
    Switch(const std::string&, const std::string&);
    Switch(const std::string&, const char&, const std::string&);
    Switch(const Switch&);
    virtual ~Switch();

    Switch* clone() const { return new Switch(*this); };

    Switch& required  (const bool& = true);
    Switch& help      (const std::string&);
    Switch& defaultVal(const bool&);

    void parse(Object&,
               std::vector<std::list<std::string>>&,
               std::vector<std::list<std::string>>&) const;

    void noParsed(Object&) const;

private:
    bool hasDefault_;
    bool default_;

    void initDefault_();
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_SWITCH_H_ */
