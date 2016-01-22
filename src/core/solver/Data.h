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

#ifndef SEMBA_SOLVER_DATA_H_
#define SEMBA_SOLVER_DATA_H_

#include "Options.h"

namespace SEMBA {
namespace Solver {

class Data {
public:
    Data() {}
    Data(const std::string& name) {
        solverName_ = name;
    }
    Data(const std::string& name, const Options& opts) {
        solverName_ = name;
        solverOpts_ = opts;
    }
    Data(const Data& rhs) {
        solverName_ = rhs.solverName_;
        solverOpts_ = rhs.solverOpts_;
    }
    Data::Data(Data&& rhs) {
        solverName_ = std::move(rhs.solverName_);
        solverOpts_ = std::move(rhs.solverOpts_);
    }
    virtual ~Data() {}

    const std::string& getName   () const { return solverName_; }
    const Options&     getOptions() const { return solverOpts_; }

    void setName   (const std::string& name) { solverName_ = name; }
    void setOptions(const Options&     opts) { solverOpts_ = opts; }

    Data operator+ (const Options& opts) {
        return Data(getName(), getOptions() + opts);
    }

    template<typename T>
    bool is() const {
        T aux;
        return aux.parse(*this);
    }
    template<typename T>
    T castTo() const {
        T res;
        if (!res.parse(*this)) {
            throw std::bad_cast();
        }
        return res;
    }

    void printInfo() const {
        std::cout << "--- Solver Data ---" << std::endl;
        std::cout << "Solver name: " << solverName_ << std::endl;
        std::cout << "Solver options:" << std::endl;
        solverOpts_.printInfo();
    }

private:
    std::string solverName_;
    Options     solverOpts_;
};

} /* namespace Solver */
} /* namespace SEMBA */

#endif /* SEMBA_SOLVER_DATA_H_ */
