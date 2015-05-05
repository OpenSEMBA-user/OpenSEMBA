#include "ClassBase.h"

ClassBase::ErrorCast::ErrorCast(const type_info& orig, const type_info& dest)
:   orig_(orig), dest_(dest){
    setMsg(string("ClassBase: Error casting ") + orig.name() +
           string(" to ") + dest.name());
}

ClassBase::ErrorCast::~ErrorCast() throw() {

}

const type_info& ClassBase::ErrorCast::getTypeOrig() const {
    return orig_;
}

const type_info& ClassBase::ErrorCast::getTypeDest() const {
    return dest_;
}

ClassBase::ClassBase() {

}

ClassBase::~ClassBase() {

}

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
