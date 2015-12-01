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

#ifndef SEMBA_GEOMETRY_ELEMENT_VOLUME_H_
#define SEMBA_GEOMETRY_ELEMENT_VOLUME_H_

#include <exception>

#include "math/simplex/Simplex.h"
#include "Surface.h"

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class VolumeBase : public virtual Base {
public:
    VolumeBase() {};
    virtual ~VolumeBase() {};
};

template<class T>
class Volume : public virtual Element<T>,
               public virtual VolumeBase {
public:
    Volume();
    virtual ~Volume();

    bool isLocalFace(const Size f,
                     const Surface<T>& surf) const;
    virtual bool isCurvedFace(const Size face) const = 0;
    virtual bool isFaceContainedInPlane(
            const Size face,
            const Math::Constants::CartesianPlane plane) const;

    Math::Vector::Cartesian<T,3> getSideNormal(const Size f) const;

    virtual Math::Real getAreaOfFace(const Size face) const = 0;
    virtual Math::Real getVolume() const = 0;
    Box<T,3> getBoundOfFace(const Size face) const;
    Size getFaceNumber(const Surface<T>*) const;
};

namespace Error {

class NullVolume : public Error {
public:
    NullVolume(const Id& elemId) : elemId_(elemId) {
        std::stringstream aux;
        aux << "Element (" << elemId_ << ") has null volume";
        str_ = aux.str();
    }
    ~NullVolume() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    Id elemId_;
    std::string str_;
};

class SurfNotFound : public Error {
public:
    SurfNotFound(const Id& volId, const Id& surfId)
    :   elemId_(volId), surfId_(surfId) {
        std::stringstream aux;
        aux << "Surf " << surfId_ << " is not part of Volume " << elemId_;
        str_ = aux.str();
    }
    ~SurfNotFound() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    Id elemId_, surfId_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Element */

typedef Element::VolumeBase         Vol;
typedef Element::Volume<Math::Real> VolR;
typedef Element::Volume<Math::Int > VolI;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_ELEMENT_VOLUME_H_ */
