#include "Identifiable.h"

namespace SEMBA {
namespace Class {

template<class Id>
Identifiable<Id>::Identifiable(const Id& id) {
    id_ = id;
}

template<class Id>
Id Identifiable<Id>::getId() const {
    return this->id_;
}

template<class Id>
void Identifiable<Id>::setId(const Id& id) {
    this->id_ = id;
}

template<class Id>
bool Identifiable<Id>::operator==(const Identifiable& rhs) const {
    return (this->id_ == rhs.id_);
}

} /* namespace Class */
} /* namespace SEMBA */
