

#include "Group.h"

namespace SEMBA {
namespace Source {

template<typename S>
Group<S>& Group<S>::operator=(SEMBA::Group::Group<S>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Group<S>::operator=(rhs);
    return *this;
}

template<typename S>
Group<S>& Group<S>::operator=(SEMBA::Group::Group<S>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Group<S>::operator=(std::move(rhs));
    return *this;
}

} /* namespace Source */
} /* namespace SEMBA */
