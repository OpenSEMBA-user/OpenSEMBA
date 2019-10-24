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

#ifndef SEMBA_GEOMETRY_ELEMENT_QUADRILATERAL4_H_
#define SEMBA_GEOMETRY_ELEMENT_QUADRILATERAL4_H_

#include "Quadrilateral.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Quadrilateral4Base : public virtual SurfaceBase {
public:
    static const std::size_t sizeOfCoordinates = 4;

    Quadrilateral4Base() {}
    virtual ~Quadrilateral4Base() {}

    std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const { 
        return 2; 
    }
};

template<class T>
class Quadrilateral4: public virtual Quadrilateral<T>,
                      public virtual Quadrilateral4Base {
public:
    Quadrilateral4();
    Quadrilateral4(const Id id,
                   const Coordinate::Coordinate<T,3>* coords[4],
                   const Layer* lay = nullptr,
                   const Model* mat = nullptr);
	Quadrilateral4(const Id id,
		std::array<const Coordinate::Coordinate<T, 3>*, 4> v,
		const Layer* lay,
		const Model* mat);
    Quadrilateral4(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                   const Id id,
                   const Box<T,3>& box,
                   const Layer* lay = nullptr,
                   const Model* mat = nullptr);
    Quadrilateral4(const Quadrilateral4<T>& rhs);
    virtual ~Quadrilateral4();

    SEMBA_CLASS_DEFINE_CLONE(Quadrilateral4<T>);

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>*);

    ElemI* toStructured(const Coordinate::Group<CoordI3>&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;
    void check() const;

private:
    const Coordinate::Coordinate<T,3>* v_[4];
};

} /* namespace Element */

typedef Element::Quadrilateral4Base         Qua4;
typedef Element::Quadrilateral4<Math::Real> QuaR4;
typedef Element::Quadrilateral4<Math::Int > QuaI4;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Quadrilateral4.hpp"

#endif /* SEMBA_GEOMETRY_ELEMENT_QUADRILATERAL4_H_ */
