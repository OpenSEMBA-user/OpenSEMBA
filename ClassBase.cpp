#include "ClassBase.h"

ClassBase::ClassBase() {

}

ClassBase::~ClassBase() {

}

template<typename T>
bool ClassBase::isOf() const {
    if(dynamic_cast<const T*>(this) != nullptr)
        return true;
    return false;
}

template<typename T>
T* ClassBase::castTo() {
    if(!this->isOf<T>()) {
        return NULL;
    }

    return dynamic_cast<T*>(const_cast<ClassBase*>(this));
}

template<typename T>
const T* ClassBase::castTo() const {
    if(!this->isOf<T>()) {
        return NULL;
    }

    return dynamic_cast<const T*>(this);
}
