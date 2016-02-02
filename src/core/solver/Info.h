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

#ifndef SEMBA_SOLVER_INFO_H_
#define SEMBA_SOLVER_INFO_H_

#include "argument/Object.h"

namespace SEMBA {
namespace Solver {

typedef Argument::Object Settings;

class Info {
public:
    Info() {}
    Info(const std::string& name) {
        solverName_ = name;
    }
    Info(const std::string& name, const Settings& sett) {
        solverName_ = name;
        solverSett_ = sett;
    }
    Info(const Info& rhs) {
        solverName_ = rhs.solverName_;
        solverSett_ = rhs.solverSett_;
    }
    Info(Info&& rhs) {
        solverName_ = std::move(rhs.solverName_);
        solverSett_ = std::move(rhs.solverSett_);
    }
    virtual ~Info() {}

    const std::string& getName    () const { return solverName_; }
    const Settings&    getSettings() const { return solverSett_; }

    void setName    (const std::string& name) { solverName_ = name; }
    void setSettings(const Settings&    sett) { solverSett_ = sett; }

    Info operator+ (const Settings& opts) {
        return Info(getName(), getSettings() + opts);
    }

    void printInfo() const {
        std::cout << "--- Solver Data ---" << std::endl;
        std::cout << "Solver name: " << solverName_ << std::endl;
        std::cout << "Solver settings:" << std::endl;
        solverSett_.printInfo();
    }

private:
    std::string solverName_;
    Settings    solverSett_;
};

} /* namespace Solver */
} /* namespace SEMBA */

#endif /* SEMBA_SOLVER_INFO_H_ */
