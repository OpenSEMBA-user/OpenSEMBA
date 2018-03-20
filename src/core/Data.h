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

#ifndef SEMBA_DATA_H_
#define SEMBA_DATA_H_

#include "geometry/mesh/Mesh.h"
#include "physicalModel/Group.h"
#include "outputRequest/Group.h"
#include "source/Group.h"
#include "solver/Info.h"

#include "filesystem/Project.h"
#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Printable.h"

namespace SEMBA {

class Data : public virtual FileSystem::Project,
             public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Printable {
public:
    Solver::Info*           solver;

    Geometry::Mesh::Mesh*   mesh;

    PhysicalModel::Group<>* physicalModels;

    Source::Group<>*        sources;
    OutputRequest::Group<>* outputRequests;

    Data();
    Data(const Data& rhs);
    virtual ~Data();

    SEMBA_CLASS_DEFINE_CLONE(Data);

    Data& operator=(const Data& rhs);

    void printInfo() const;
};

} /* namespace SEMBA */

#endif /* SEMBA_DATA_H_ */
