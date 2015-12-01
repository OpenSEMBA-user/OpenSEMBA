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
/*
 * Mesh.h
 *
 *  Created on: Mar 19, 2015
 *      Author: damarro
 */

#ifndef SRC_COMMON_GEOMETRY_MESH_H_
#define SRC_COMMON_GEOMETRY_MESH_H_

#include "Types.h"
#include "coordinates/GroupCoordinates.h"
#include "elements/GroupElements.h"
#include "layers/GroupLayers.h"
#include "Grid.h"

#include "base/class/ClassBase.h"

class Mesh : public virtual ClassBase {
public:
    Mesh();
    ~Mesh();

    virtual void applyScalingFactor(const Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
};

#endif /* SRC_COMMON_GEOMETRY_MESH_H_ */
