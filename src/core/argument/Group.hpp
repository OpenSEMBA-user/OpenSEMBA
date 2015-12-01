
#include "Group.h"

namespace SEMBA {
namespace Argument {

template<bool S>
Group<S>::Group(Group<true>* group,
                const std::string& name,
                const std::string& desc)
:   GroupBase(group, name, desc) {
    numMutExc_ = 0;
}

template<bool S>
Group<S>::~Group() {

}

template<bool S>
bool Group<S>::isRequired() const {
    if (isMutuallyExclusive()) {
        return GroupBase::isRequired();
    }
    return false;
}

template<bool S>
Group<S>& Group<S>::required() {
    if (isMutuallyExclusive()) {
        GroupBase::required();
    }
    return *this;
}

template<bool S> template<bool S2, typename T>
Group<true>& Group<S>::addGroup(const std::string& name,
                                const std::string& desc) {
    insertName_(name);
    Group<true>* newGroup = new Group<true>(this, name, desc);

    childName_[name] = child_.size();
    child_.push_back(newGroup);

    return *newGroup;
}

template<bool S> template<bool S2, typename T>
Group<false>& Group<S>::addMutuallyExclusiveGroup() {
    std::stringstream aux;
    aux << "__aux" << ++numMutExc_ << "__";
    std::string name = aux.str();
    insertName_(aux.str());
    Group<false>* newGroup = new Group<false>(this, name);

    childName_[name] = child_.size();
    child_.push_back(newGroup);

    return *newGroup;
}

} /* namespace Argument */
} /* namespace SEMBA */
