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
#ifndef PHYSICALMODEL_H_
#define PHYSICALMODEL_H_

#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

#include "geometry/elements/GroupElements.h"
#include "math/Constants.h"
#include "math/CartesianVector.h"
#include "ProjectFile.h"

#include "base/class/ClassIdBase.h"

class PhysicalModel : public ClassIdBase<MatId> {
public:
    enum Type {
        PEC,
        PMC,
        SMA,
        classic,
        elecDispersive,
        anisotropic,
        isotropicsibc,
        PML,
        wire,
        multiport
    };

	PhysicalModel(const MatId id, const string& name);
	virtual ~PhysicalModel();

	const string& getName() const;
	void setName(const string& newName);

	virtual void printInfo() const;
private:
    string name_;
};


#endif
