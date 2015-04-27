#include "ClassBase.h"

shared_ptr<ClassBase> ClassBase::getSharedPtr() {
    try {
        return shared_from_this();
    } catch (const bad_weak_ptr&) {
        return shared_ptr<ClassBase>(this);
    }
}

shared_ptr<const ClassBase> ClassBase::getSharedPtr() const {
    try {
        return shared_from_this();
    } catch (const bad_weak_ptr&) {
        return shared_ptr<const ClassBase>(this);
    }
}
