#include "ClassBase.h"

template<typename T>
bool ClassBase::is() const {
    if(dynamic_cast<const T*>(this) != NULL)
        return true;
    return false;
}

template<typename T> template<typename T2>
bool isSimilar(const T2& rhs) const {
    return rhs.is<T>(rhs);
}

template<typename T>
typename add_pointer<typename remove_const<T>::type>::type
    ClassBase::cloneTo() const {

    return clone()->castTo<typename remove_const<T>::type>();
}

template<typename T>
T* ClassBase::castTo() {
    if(!this->is<T>()) {
        cerr << endl << "ERROR @ ClassBase::castTo():"
        << "Invalid cast" << endl;
        assert(false);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return dynamic_cast<T*>(const_cast<ClassBase*>(this));
}

template<typename T>
const T* ClassBase::castTo() const {
    if(!this->is<T>()) {
        cerr << endl << "ERROR @ ClassBase::castTo():"
        << "Invalid cast" << endl;
        assert(false);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return dynamic_cast<const T*>(this);
}
