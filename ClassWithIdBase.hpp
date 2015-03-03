#ifndef CASSWITHIDBASE_H_
#include "ClassWithIdBase.h"
#endif

template<class Id>
ClassWithIdBase<Id>::ClassWithIdBase()
:   id_(0) {

}

template<class Id>
ClassWithIdBase<Id>::ClassWithIdBase(const Id id)
:   id_(id) {

}

template<class Id>
ClassWithIdBase<Id>::ClassWithIdBase(const ClassWithIdBase<Id>& rhs)
:   id_(rhs.id_) {

}

template<class Id>
bool ClassWithIdBase<Id>::operator==(const ClassWithIdBase<Id>& rhs) const {
    return this->id_ == rhs.id_;
}

template<class Id>
Id ClassWithIdBase<Id>::getId() const {
    return this->id_;
}

template<class Id>
void ClassWithIdBase<Id>::setId(const Id id) {
    this->id_ = id;
}

template<class Id>
ClassWithIdBase<Id>::~ClassWithIdBase() {

}
