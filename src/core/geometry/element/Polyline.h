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

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include <vector>

#include "Line.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class PolylineBase : public virtual LineBase {
public:
    PolylineBase() {};
    virtual ~PolylineBase() {};
};

template<class T>
class Polyline : public virtual Line<T>,
                 public virtual PolylineBase {
public:
    Polyline();
    Polyline(const Id id,
             const std::vector<const Coordinate::Coordinate<T,3>*>& v,
             const Layer* lay = NULL,
             const Model* mat = NULL);
    Polyline(const Polyline<T>& rhs);
    virtual ~Polyline();
    
    SEMBA_CLASS_DEFINE_CLONE(Polyline);

    inline std::size_t numberOfCoordinates() const { return v_.size(); }

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord);

    void printInfo() const;

private:
    std::vector<const Coordinate::Coordinate<T,3>*> v_;
};

} /* namespace Element */

typedef Element::PolylineBase         Polylin;
typedef Element::Polyline<Math::Real> PolylinR;
typedef Element::Polyline<Math::Int > PolylinI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Polyline.h"

#endif /* POLYLINE_H_ */
