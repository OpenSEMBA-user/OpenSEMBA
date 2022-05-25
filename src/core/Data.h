#pragma once

#include "geometry/mesh/Mesh.h"
#include "physicalModel/Group.h"
#include "outputRequest/Group.h"
#include "source/Group.h"

#include "filesystem/Project.h"
#include "class/Class.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

#include "nlohmann/json.hpp"

namespace SEMBA {

class Data;
    typedef Class::Identification<Data> Id;
    typedef nlohmann::json json;

class Data : public virtual Class::Identifiable<Id>,
             public virtual Class::Class {

public:
    FileSystem::Project     filename;
    json                    solver;
    std::unique_ptr<Geometry::Mesh::Mesh>   mesh;
    PMGroup physicalModels;
    SourceGroup        sources;
    OutputRequestGroup outputRequests;

    Data(const Data& rhs);
    virtual ~Data() = default;

    virtual std::unique_ptr<Data> clone() const {
        return std::make_unique<Data>(*this);
    }

    Data& operator=(const Data& rhs);
};

} /* namespace SEMBA */

