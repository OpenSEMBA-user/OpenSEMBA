#include "ClassBase.h"

template<typename T>
bool ClassBase::is() const {
    if(dynamic_cast<const T*>(this) != NULL) {
        return true;
    }
    return false;
}

template<typename T>
T* ClassBase::cloneTo() const {
    return clone()->castTo<T>();
}

template<typename T>
T* ClassBase::castTo() {
    return &dynamic_cast<T&>(*this);
}

template<typename T>
const T* ClassBase::castTo() const {
    return &dynamic_cast<const T&>(*this);
}
