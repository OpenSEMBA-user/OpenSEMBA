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
 * Volume.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef VOLUME_H_
#define VOLUME_H_

#include "base/error/Error.h"
#include "math/Simplex.h"

#include "Surface.h"

#include "Element.h"

class VolumeBase : public virtual ElementBase {
public:
    VolumeBase() {};
    virtual ~VolumeBase() {};
};

template<class T>
class Volume : public virtual Element<T>,
               public virtual VolumeBase {
public:
    class ErrorNullVolume : public Error {
    public:
        ErrorNullVolume(const ElementId& elemId);
        ~ErrorNullVolume() throw();
    };

    class ErrorSurfNotFound : public Error {
    public:
        ErrorSurfNotFound(const ElementId& volId, const ElementId& surfId);
        ~ErrorSurfNotFound() throw();
    };

	Volume();
    virtual ~Volume();

	bool isLocalFace(const UInt f,
                     const Surface<T>& surf) const;
	virtual bool isCurvedFace(const UInt face) const = 0;
    virtual bool isFaceContainedInPlane(const UInt face,
                                        const CartesianPlane plane) const;

    CartesianVector<T,3> sideNormal(const UInt f) const;

    virtual Real getAreaOfFace(const UInt face) const = 0;
    virtual Real getVolume() const = 0;
    Box<T,3> getBoundOfFace(const UInt face) const;
	UInt getFaceNumber(const Surface<T>*) const;
};

typedef VolumeBase   Vol;
typedef Volume<Real> VolR;
typedef Volume<Int> VolI;

#endif /* VOLUME_H_ */
