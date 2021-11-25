

#pragma once

#include "math/matrix/Static.h"
#include "filesystem/Project.h"

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class SIBCFile : public virtual Surface {
public:
    SIBCFile();
    SIBCFile(const Id id,
             const std::string& name,
             const FileSystem::Project& file);
    
    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<SIBCFile>(*this);
    }

    const FileSystem::Project getFile() const;

protected:
    FileSystem::Project file_;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

