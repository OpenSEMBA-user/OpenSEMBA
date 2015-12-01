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
 * PhysicalModelGroup.h
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef COMMON_PHYSICALMODEL_GROUPPHYSICALMODELS_H_
#define COMMON_PHYSICALMODEL_GROUPPHYSICALMODELS_H_

#include <cassert>
#include <cmath>
#include <vector>
using namespace std;

#include "PhysicalModel.h"
#include "PMPEC.h"
#include "PMPMC.h"
#include "PMSMA.h"
#include "PMVolumeClassic.h"
#include "PMVolumeDispersive.h"
#include "PMVolumeAnisotropic.h"
#include "PMVolumeAnisotropicCrystal.h"
#include "PMVolumeAnisotropicFerrite.h"
#include "PMVolumePML.h"
#include "PMMultiportPredefined.h"
#include "PMMultiportRLC.h"
#include "PMSurfaceSIBC.h"
#include "PMSurfaceMultilayer.h"
#include "PMWire.h"
#include "PMWireExtremes.h"

#include "base/group/GroupId.h"

template<typename P = PhysicalModel>
class GroupPhysicalModels : public GroupId<P,MatId> {
public:
    GroupPhysicalModels() {}
    template<typename P2>
    GroupPhysicalModels(P2* e)                     : GroupId<P,MatId>(e) {}
    template<typename P2>
    GroupPhysicalModels(const std::vector<P2*>& e) : GroupId<P,MatId>(e) {}
    template<typename P2>
    GroupPhysicalModels(VectorPtr<P2>&       rhs) : GroupId<P,MatId>(rhs) {}
    template<typename P2>
    GroupPhysicalModels(const VectorPtr<P2>& rhs) : GroupId<P,MatId>(rhs) {}
    GroupPhysicalModels(VectorPtr<P>&        rhs) : GroupId<P,MatId>(rhs) {}
    template<typename P2>
    GroupPhysicalModels(VectorPtr<P2>&& rhs) : GroupId<P,MatId>(std::move(rhs)) {}
    GroupPhysicalModels(VectorPtr<P >&& rhs) : GroupId<P,MatId>(std::move(rhs)) {}
    virtual ~GroupPhysicalModels() {}

    DEFINE_GROUP_CLONE(GroupPhysicalModels, P);

    GroupPhysicalModels& operator=(VectorPtr<P>&);
    GroupPhysicalModels& operator=(VectorPtr<P>&&);

    void printInfo() const;
};

#include "GroupPhysicalModels.hpp"

typedef GroupPhysicalModels<> PMGroup;

#endif /* COMMON_PHYSICALMODEL_GROUPPHYSICALMODELS_H_ */
