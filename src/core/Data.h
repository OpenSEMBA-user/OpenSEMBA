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
 * SmbData.h
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#ifndef SMBDATA_H_
#define SMBDATA_H_

#include "geometry/MeshUnstructured.h"
#include "geometry/MeshStructured.h"
#include "geometry/Grid.h"
#include "physicalModel/GroupPhysicalModels.h"
#include "options/OptionsMesher.h"
#include "options/OptionsSolverFDTD.h"
#include "options/OptionsSolverDGTD.h"
#include "outRq/GroupOutRqs.h"
#include "ProjectFile.h"
#include "base/class/ClassBase.h"
#include "sources/GroupEMSources.h"

class SmbData : public virtual ProjectFile,
                public virtual ClassBase {
public:
    Grid3*                 grid;
    Mesh*                  mesh;

    OptionsMesher*         mesherOptions;
    OptionsSolver*         solverOptions;

    GroupPhysicalModels<>* pMGroup;

    GroupEMSources<>*      emSources;
    GroupOutRqs<>*         outputRequests;

    SmbData();
    SmbData(const SmbData& rhs);
    virtual ~SmbData();

    DEFINE_CLONE(SmbData);

    SmbData& operator=(const SmbData& rhs);

    bool check() const;

    void printInfo() const;

private:
};

#endif /* SMBDATA_H_ */
