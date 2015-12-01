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

#ifndef SEMBA_SOURCE_ONLINE_H_
#define SEMBA_SOURCE_ONLINE_H_

#include "Source.h"

namespace SEMBA {
namespace Source {

class OnLine : public Source<Geometry::Lin> {
public:
    enum Type {
        electric = 1,
        magnetic = 2
    };
    enum Hardness {
        hard = 1,
        soft = 2
    };
    OnLine(const Magnitude::Magnitude* magnitude,
           const Geometry::Element::Group<Geometry::Lin>& elem,
           const Type& sourceType,
           const Hardness& sourceHardness);
    OnLine(const OnLine& rhs);
    virtual ~OnLine();

    SEMBA_CLASS_DEFINE_CLONE(OnLine);

    bool hasSameProperties(const SEMBA::Source::Base& rhs) const;

    const std::string& getName() const;
    Type getType() const;
    Hardness getHardness() const;

    virtual void printInfo() const;
private:
    Type type_;
    Hardness hardness_;

    std::string getTypeStr() const;
    std::string getHardnessStr() const;
};

} /* namespace Source */
} /* namespace SEMBA */

#endif /* SEMBA_SOURCE_ONLINE_H_ */
