#ifndef CASSIDBASE_H_
#include "ClassIdBase.h"
#endif

template<class Id>
ClassIdBase<Id>::ClassIdBase()
:   id_(0) {

}

template<class Id>
ClassIdBase<Id>::ClassIdBase(const Id id)
:   id_(id) {

}

template<class Id>
ClassIdBase<Id>::ClassIdBase(const ClassIdBase<Id>& rhs)
:   id_(rhs.id_) {

}

template<class Id>
bool ClassIdBase<Id>::operator==(const ClassIdBase<Id>& rhs) const {
    return this->id_ == rhs.id_;
}

template<class Id>
bool ClassIdBase<Id>::operator!=(const ClassIdBase<Id>& rhs) const {
    return this->id_ != rhs.id_;
}

template<class Id>
Id ClassIdBase<Id>::getId() const {
    return this->id_;
}

template<class Id>
void ClassIdBase<Id>::setId(const Id id) {
    this->id_ = id;
}

template<class Id>
ClassIdBase<Id>::~ClassIdBase() {

}
