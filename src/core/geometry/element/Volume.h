

#pragma once

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

    bool isLocalFace(const std::size_t f,
                     const Surface<T>& surf) const;
    virtual bool isCurvedFace(const std::size_t face) const = 0;
    virtual bool isFaceContainedInPlane(
            const std::size_t face,
            const Math::Constants::CartesianPlane plane) const;

    Math::Vector::Cartesian<T,3> getSideNormal(const std::size_t f) const;

    virtual Math::Real getAreaOfFace(const std::size_t face) const = 0;
    virtual Math::Real getVolume() const = 0;
    Box<T,3> getBoundOfFace(const std::size_t face) const;
    std::size_t getFaceNumber(const Surface<T>*) const;
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

#include "Volume.hpp"

