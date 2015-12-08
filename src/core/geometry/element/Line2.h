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

#ifndef SEMBA_GEOMETRY_ELEMENT_LINE2_H_
#define SEMBA_GEOMETRY_ELEMENT_LINE2_H_

#include "Line.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Line2Base : public virtual LineBase {
public:
    Line2Base() {};
    virtual ~Line2Base() {};

    inline std::size_t numberOfCoordinates() const { return 2; }
};

template<class T>
class Line2 : public virtual Line<T>,
              public virtual Line2Base {
public:
    Line2();
    Line2(const Id id,
          const Coordinate::Coordinate<T,3>* v[2],
          const Layer* lay = NULL,
          const Model* mat = NULL);
    Line2(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
          const Id id,
          const Box<T,3>& box,
          const Layer* lay = NULL,
          const Model* mat = NULL);
    Line2(const Coordinate::Coordinate<T,3>* v[2]);
    Line2(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
          const Box<T,3>& box);
    Line2(const Line2<T>& rhs);
    virtual ~Line2();
    
    SEMBA_CLASS_DEFINE_CLONE(Line2<T>);

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord);

    ElemI* toStructured(const Coordinate::Group<CoordI3>&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;

private:
    static const Math::Simplex::Line<1> lin;
    const Coordinate::Coordinate<T,3>* v_[2];

    void setCoordinates(const Coordinate::Coordinate<T,3>* v[2]);
    void setCoordinates(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                        const Box<T,3>& box);
};

} /* namespace Element */

typedef Element::Line2Base         Lin2;
typedef Element::Line2<Math::Real> LinR2;
typedef Element::Line2<Math::Int > LinI2;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Line2.hpp"

#endif /* SEMBA_GEOMETRY_ELEMENT_LINE2_H_ */
