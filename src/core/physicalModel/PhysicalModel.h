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

#ifndef SEMBA_PHYSICALMODEL_PHYSICALMODEL_H_
#define SEMBA_PHYSICALMODEL_PHYSICALMODEL_H_

#include "math/Constants.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {
namespace PhysicalModel {

class PhysicalModel;
typedef Class::Identification<PhysicalModel> Id;

class PhysicalModel : public virtual Class::Class,
                      public virtual Class::Cloneable,
                      public virtual Class::Shareable,
                      public virtual Class::Printable,
                      public Class::Identifiable<Id> {
public:
    enum Type {
        PEC,
        PMC,
        SMA,
        classic,
        elecDispersive,
        anisotropic,
        isotropicsibc,
        PML,
        wire,
        multiport
    };

    PhysicalModel(const Id id, const std::string& name);
    virtual ~PhysicalModel();

    const std::string& getName() const;
    void setName(const std::string& newName);

    virtual void printInfo() const;

private:
    std::string name_;
};

namespace Error {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

} /* namespace Error */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_PHYSICALMODEL_H_ */
