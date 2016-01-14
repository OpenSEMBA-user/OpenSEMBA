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
 * OpenfoamBoundary.h
 *
 *  Created on: Apr 11, 2014
 *      Author: luis
 */

#ifndef OPENFOAMBOUNDARY_H_
#define OPENFOAMBOUNDARY_H_

#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

using namespace std;

#include "geometry/layers/Layer.h"
#include "physicalModel/PhysicalModel.h"
#include "Types.h"

class OpenfoamBoundary : public Layer {
    friend class ParserOpenFoam;
public:
	OpenfoamBoundary();
	OpenfoamBoundary(
	 const string name,
	 const UInt nFaces,
	 const UInt startFace);
	virtual ~OpenfoamBoundary();
	UInt getFaces() const;
	UInt getStartFace() const;
    void printInfo() const;
protected:
    bool isMaterial() const;
    MatId getMaterialIdFromName() const;
private:
	UInt nFaces_;
	UInt startFace_;
	Int strpos(const char *haystack, const char *needle, Int nth) const;
    LayerId getLayerIdFromName() const;
};

#endif /* OPENFOAMBOUNDARY_H_ */
