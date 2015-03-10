#include "IdBase.h"

IdBase::IdBase()
:   id_(0) {

}

IdBase::IdBase(const UInt id)
:   id_(id) {

}

IdBase::IdBase(const IdBase& rhs)
:   id_(rhs.id_) {

}

IdBase::~IdBase() {
    
}

IdBase::operator UInt() const {
    return id_;
}
