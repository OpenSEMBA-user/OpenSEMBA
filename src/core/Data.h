

#pragma once

#include "geometry/mesh/Mesh.h"
#include "physicalModel/Group.h"
#include "outputRequest/Group.h"
#include "source/Group.h"
#include "solver/Info.h"

#include "filesystem/Project.h"
#include "class/Class.h"
#include "class/Cloneable.h"

namespace SEMBA {

class Data : public virtual Class::Class,
             public virtual Class::Cloneable {
public:
    FileSystem::Project     filename;
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
};

} /* namespace SEMBA */

