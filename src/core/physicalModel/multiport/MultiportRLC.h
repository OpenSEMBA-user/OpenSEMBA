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

#ifndef SEMBA_PHYSICALMODEL_MULTIPORTRLC_H_
#define SEMBA_PHYSICALMODEL_MULTIPORTRLC_H_

#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {

class MultiportRLC : public virtual Multiport {
public:
    MultiportRLC(const Id idIn,
                 const std::string nameIn,
                 const Multiport::Type typeIn,
                 const Math::Real resistance,
                 const Math::Real inductance,
                 const Math::Real capacitance);
    MultiportRLC(const MultiportRLC&);
    virtual ~MultiportRLC();

    SEMBA_CLASS_DEFINE_CLONE(MultiportRLC);

    virtual Math::Real getR() const;
    virtual Math::Real getL() const;
    virtual Math::Real getC() const;
    virtual void printInfo() const;
private:
    Math::Real R_, L_, C_;
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_MULTIPORTRLC_H_ */
