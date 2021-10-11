

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
    
    SEMBA_CLASS_DEFINE_CLONE(SIBCFile);

    const FileSystem::Project getFile() const;

protected:
    FileSystem::Project file_;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

