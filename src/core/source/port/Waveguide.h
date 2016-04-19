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

#ifndef SEMBA_SOURCE_PORT_WAVEPORT_H_
#define SEMBA_SOURCE_PORT_WAVEPORT_H_

#include "Port.h"
#include "geometry/BoundTerminations.h"

namespace SEMBA {
namespace Source {
namespace Port {

class Waveguide : public Port {
public:
    typedef enum {
        TE,
        TM
    } ExcitationMode;

    Waveguide(Magnitude::Magnitude* magnitude,
              const Geometry::Element::Group<const Geometry::Surf>& elem,
              const ExcitationMode excMode,
              const std::pair<size_t,size_t> mode,
              const Geometry::BoundTerminations3 bounds);
    Waveguide(const Waveguide& rhs);
    virtual ~Waveguide();

    bool hasSameProperties(const SEMBA::Source::Base& rhs) const;

    ExcitationMode getExcitationMode() const;
    std::pair<size_t, size_t> getMode() const;

    void printInfo() const;
private:
    ExcitationMode excitationMode_;
    std::pair<size_t,size_t> mode_;
    Geometry::BoundTerminations3 bounds_;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

#endif /* SEMBA_SOURCE_PORT_WAVEPORT_H_ */
