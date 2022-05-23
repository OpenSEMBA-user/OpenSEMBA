#pragma once

#include "geometry/mesh/Mesh.h"
#include "physicalModel/Group.h"
#include "outputRequest/Group.h"
#include "source/Group.h"

#include "filesystem/Project.h"
#include "class/Class.h"
#include "class/Cloneable.h"

#include "nlohmann/json.hpp"

namespace SEMBA {

class Data : public virtual Class::Class,
             public virtual Class::Cloneable {
public:
    typedef nlohmann::json json;

    FileSystem::Project     filename;
    json                    solver;
    Geometry::Mesh::Mesh*   mesh;
    PMGroup physicalModels;
    Source::Group<>*        sources;
    OutputRequest::Group<>* outputRequests;

    Data();
    Data(const Data& rhs);
    virtual ~Data();

    SEMBA_CLASS_DEFINE_CLONE(Data);

    Data& operator=(const Data& rhs);
};

} /* namespace SEMBA */

