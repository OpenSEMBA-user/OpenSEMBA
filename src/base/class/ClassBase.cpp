#include "ClassBase.h"

ClassBase::ClassBase() {

}

ClassBase::~ClassBase() {

}

std::shared_ptr<ClassBase> ClassBase::getSharedPtr() {
    try {
        return shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        return std::shared_ptr<ClassBase>(this);
    }
}

std::shared_ptr<const ClassBase> ClassBase::getSharedPtr() const {
    try {
        return shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        return std::shared_ptr<const ClassBase>(this);
    }
}
