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
 * Lin2Conformal.h
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_
#define SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_

#include <geometry/elements/Line2.h>
#include "geometry/coordinates/CoordinateConformal.h"


class LineConformal : public virtual Line2<Int> {
public:
    class ErrorCoordNotConf : public Element<Int>::ErrorCoord {
    public:
        ErrorCoordNotConf(const CoordinateId& coordId);
        ~ErrorCoordNotConf() throw();
    };

    LineConformal();
    LineConformal(const ElementId id,
                  const CoordI3* v[2],
                  const CVecR3& norm,
                  const LayerId layerId = LayerId(0),
                  const MatId   matId   = MatId(0));
    LineConformal(const CoordI3* v[2],
                  const CVecR3& norm,
                  const LayerId layerId = LayerId(0),
                  const MatId   matId   = MatId(0));
    LineConformal(const LineConformal& rhs);
    virtual ~LineConformal();

    DEFINE_CLONE(LineConformal);

    CVecR3 getNorm () const { return norm_;  }

    const CoordConf* getConfV(const UInt i) const;

    void setV(const UInt i, const CoordI3* coord);

    ElemR* toUnstructured(const GroupCoordinates<CoordR3>&, const Grid3&) const;

    void printInfo() const;

private:
    void checkCoordinates();
    CVecR3 norm_;
};

typedef LineConformal LinConf;

#endif /* SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_ */
