

#include "Group.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P>
Group<P>& Group<P>::operator=(SEMBA::Group::Group<P>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<P, Id>::operator=(rhs);
    return *this;
}

template<typename P>
Group<P>& Group<P>::operator=(SEMBA::Group::Group<P>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<P, Id>::operator=(std::move(rhs));
    return *this;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
