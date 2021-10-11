

#include <physicalModel/surface/SIBCFile.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

SIBCFile::SIBCFile(const Id id,
                         const std::string& name,
                         const FileSystem::Project& file)
:   Identifiable<Id>(id),
    PhysicalModel(name),
    file_(file) {
    std::string extension = file_.getExtension();
    if (extension.compare(".mibc") != 0) {
        throw std::logic_error("File extension must be .mibc in file: " + file_);
    }
}

const FileSystem::Project SIBCFile::getFile() const {
    return file_;
}

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
