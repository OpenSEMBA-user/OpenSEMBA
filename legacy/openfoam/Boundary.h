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

#ifndef SEMBA_MESHER_OPENFOAM_BOUNDARY_H_
#define SEMBA_MESHER_OPENFOAM_BOUNDARY_H_

#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

#include "geometry/element/Element.h"
#include "geometry/layer/Layer.h"
#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace Mesher {
namespace OpenFOAM {

class Boundary : public Geometry::Layer::Layer {
    friend class Parser;
public:
	Boundary();
	Boundary(
	 const std::string name,
	 const std::size_t nFaces,
     const std::size_t startFace);
	virtual ~Boundary();
    std::size_t getFaces() const;
    std::size_t getStartFace() const;
    void printInfo() const;
protected:
    bool isMaterial() const;
    MatId getMaterialIdFromName() const;
private:
	std::size_t nFaces_;
    std::size_t startFace_;
    int strpos(const char *haystack, const char *needle, int nth) const;
    Geometry::LayerId getLayerIdFromName() const;
};

}
}
}

#endif /* SEMBA_MESHER_OPENFOAM_BOUNDARY_H_ */
