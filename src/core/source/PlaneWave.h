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

#ifndef SEMBA_SOURCE_PLANEWAVE_H_
#define SEMBA_SOURCE_PLANEWAVE_H_

#include <exception>

#include "Source.h"

namespace SEMBA {
namespace Source {

class PlaneWave : public Source<Geometry::Vol> {
public:
    PlaneWave();
    PlaneWave(Magnitude::Magnitude* magnitude,
              Geometry::Element::Group<Geometry::Vol> elem,
              Math::CVecR3 direction,
              Math::CVecR3 polarization);
    PlaneWave(const PlaneWave& rhs);
    virtual ~PlaneWave();

    SEMBA_CLASS_DEFINE_CLONE(PlaneWave);

    bool hasSameProperties(const SEMBA::Source::Base& rhs) const;

    const std::string& getName() const;
    const Math::CVecR3& getPolarization() const;
    const Math::CVecR3& getWaveDirection() const;
    Math::Real getTheta() const;
    Math::Real getPhi() const;
    Math::Real getAlpha() const;
    Math::Real getBeta() const;
    Math::CVecR3 getElectricField(const Math::Real time) const;
    std::pair<Math::CVecR3,Math::CVecR3> getElectromagneticField(
            const Math::Real time) const;

    void printInfo() const;

private:
    Math::CVecR3 direction_;
    Math::CVecR3 polarization_;

    std::pair<Math::Real,Math::Real> cartesianToPolar(
            const Math::CVecR3& vec) const;
    Math::Real reduceRadians(const Math::Real radianIn) const;
};

namespace Error {
namespace PlaneWave {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

class ZeroPolarization : public Error {
public:
    ZeroPolarization() {}
    virtual ~ZeroPolarization() throw() {}

    const char* what() const throw() {
        return "PlaneWave: Polarization can't be zero.";
    }
};

class ZeroMagnitude : public Error {
public:
    ZeroMagnitude() {}
    virtual ~ZeroMagnitude() throw() {}

    const char* what() const throw() {
        return "PlaneWave: W. direction can't be zero.";
    }
};

class NotPerpendicular : public Error {
public:
    NotPerpendicular() {}
    virtual ~NotPerpendicular() throw() {}

    const char* what() const throw() {
        return ("PlaneWave: W. direction is not "
                "perpendicular to polarization.");
    }
};

} /* namespace PlaneWave */
} /* namespace Error */
} /* namespace Source */
} /* namespace SEMBA */

#endif /* SEMBA_SOURCE_PLANEWAVE_H_ */
