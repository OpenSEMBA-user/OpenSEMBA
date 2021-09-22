

#include "Group.h"

namespace SEMBA {
namespace OutputRequest {

template<typename O>
Group<O>& Group<O>::operator=(SEMBA::Group::Group<O>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Group<O>::operator=(rhs);
    return *this;
}

template<typename O>
Group<O>& Group<O>::operator=(SEMBA::Group::Group<O>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Group<O>::operator=(std::move(rhs));
    return *this;
}

template<typename O>
void Group<O>::printInfo() const {
    std::cout<< " --- OutputRequestGroup info ---" << std::endl;
    SEMBA::Group::Printable<O>::printInfo();
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
