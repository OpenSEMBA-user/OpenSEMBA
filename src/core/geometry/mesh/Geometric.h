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

#ifndef SEMBA_GEOMETRY_MESH_GEOMETRIC_H_
#define SEMBA_GEOMETRY_MESH_GEOMETRIC_H_

#include "Unstructured.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Geometric : public Unstructured {
public:
    Geometric();
    Geometric(const Grid3& grid);
    Geometric(const Grid3& grid,
              const Coordinate::Group<const CoordR3>& cG,
              const Element::Group<const ElemR>& elem,
              const Layer::Group<const Layer::Layer>& =
                   Layer::Group<const Layer::Layer>());
    Geometric(const Geometric& param);
    virtual ~Geometric();

    Geometric& operator=(const Geometric& rhs);

    SEMBA_CLASS_DEFINE_CLONE(Geometric);

    Grid3&       grid()       { return grid_; }
    const Grid3& grid() const { return grid_; }

    Structured* getMeshStructured(const Math::Real = Grid3::tolerance) const;

    void applyScalingFactor(const Math::Real factor);

    void printInfo() const;

private:
	Grid3 grid_;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_MESH_GEOMETRIC_H_ */
