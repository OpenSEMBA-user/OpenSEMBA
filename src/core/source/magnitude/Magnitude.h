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

#include "math/function/Function.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Magnitude : public virtual Class::Class,
                  public virtual Class::Cloneable,
                  public virtual Class::Shareable,
                  public virtual Class::Printable {
public:
    Magnitude();
    Magnitude(Math::FunctionRR* mathFunction);
    Magnitude(const Magnitude& rhs);
    virtual ~Magnitude();

    SEMBA_CLASS_DEFINE_CLONE(Magnitude);

    Magnitude& operator=(const Magnitude& rhs);

    virtual bool operator==(const Magnitude&) const;

    Math::Real evaluate(const Math::Real time) const;

    virtual void printInfo() const;

private:
    Math::FunctionRR* mathFunction_;
};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

