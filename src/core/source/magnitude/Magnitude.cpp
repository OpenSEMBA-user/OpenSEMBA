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

#include "Magnitude.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

Magnitude::Magnitude() {
    mathFunction_ = NULL;
}

Magnitude::Magnitude(Math::FunctionRR* mathFunction) {
    mathFunction_ = mathFunction;
}

Magnitude::Magnitude(const Magnitude& rhs) {
    mathFunction_ = dynamic_cast<Math::FunctionRR*>(rhs.mathFunction_->clone());
}

Magnitude::~Magnitude() {
    if (mathFunction_ != NULL) {
        delete mathFunction_;
    }
}

Magnitude& Magnitude::operator=(const Magnitude& rhs) {
    if (this == &rhs) {
        return *this;
    }
    mathFunction_ = dynamic_cast<Math::FunctionRR*>(rhs.mathFunction_->clone());
    return *this;
}

bool Magnitude::operator ==(const Magnitude& rhs) const {
    return *mathFunction_ == *rhs.mathFunction_;
}

void Magnitude::printInfo() const {
    std::cout << " --- Magnitude ---" << std::endl;
    mathFunction_->printInfo();
}

Math::Real Magnitude::evaluate(const Math::Real time) const {
    return mathFunction_->operator()(time);
}

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */
