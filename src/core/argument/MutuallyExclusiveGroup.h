

#pragma once

#include "GroupBase.h"

namespace SEMBA {
namespace Argument {

class MutuallyExclusiveGroup : public GroupBase {
    friend class Group;
public:
    virtual ~MutuallyExclusiveGroup();
    
    bool isMutuallyExclusive() const { return true; }

protected:
    MutuallyExclusiveGroup(GroupBase* = nullptr,
                           const std::string& = std::string(),
                           const std::string& = std::string());

private:
    //Erased
    MutuallyExclusiveGroup(const MutuallyExclusiveGroup&);
    MutuallyExclusiveGroup(MutuallyExclusiveGroup&&);

    MutuallyExclusiveGroup& operator=(const MutuallyExclusiveGroup&);
    MutuallyExclusiveGroup& operator=(MutuallyExclusiveGroup&&);
};

typedef MutuallyExclusiveGroup MEGroup;

} /* namespace Argument */
} /* namespace SEMBA */

