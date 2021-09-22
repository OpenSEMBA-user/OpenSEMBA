

#include "Group.h"

namespace SEMBA {
namespace Argument {

MutuallyExclusiveGroup::MutuallyExclusiveGroup(GroupBase* group,
                                               const std::string& name,
                                               const std::string& desc)
:   GroupBase(group, name, desc) {

}

MutuallyExclusiveGroup::~MutuallyExclusiveGroup() {

}

} /* namespace Argument */
} /* namespace SEMBA */
