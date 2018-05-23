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

#ifndef SEMBA_PHYSICALMODEL_SURFACEMULTILAYER_H_
#define SEMBA_PHYSICALMODEL_SURFACEMULTILAYER_H_

#include <cassert>
#include <exception>
#include <vector>

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class Multilayer : public virtual Surface {
public:
    Multilayer(const Id id,
            const std::string& name,
            const std::vector<Math::Real>& thickness,
            const std::vector<Math::Real>& relPermittivity,
            const std::vector<Math::Real>& relPermeability,
            const std::vector<Math::Real>& elecCond);
    Multilayer(const Multilayer&);
    virtual ~Multilayer();

    SEMBA_CLASS_DEFINE_CLONE(Multilayer);

    std::size_t getNumberOfLayers() const;
    std::string printLayer(const std::size_t i) const;
    Math::Real getThickness(const std::size_t i) const;
    Math::Real getPermittivity(const std::size_t i) const;
    Math::Real getPermeability(const std::size_t i) const;
    Math::Real getElecCond(const std::size_t i) const;

    void printInfo() const;

private:
    std::vector<Math::Real> thickness_;
    std::vector<Math::Real> relPermittivity_;
    std::vector<Math::Real> relPermeability_;
    std::vector<Math::Real> elecCond_;
};

namespace Error {
namespace SurfaceMultilayer {

class IncompatibleSizes : public std::exception {
public:
    IncompatibleSizes() {}
    virtual ~IncompatibleSizes() throw() {}

    const char* what() const throw() {
        return "SurfaceMultilayer: "
                "Incompatible sizes of layers parameters.";
    }
};

} /* namespace SurfaceMultilayer */
} /* namespace Error */
} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_SURFACEMULTILAYER_H_ */
