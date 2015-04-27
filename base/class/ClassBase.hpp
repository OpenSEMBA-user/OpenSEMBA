#include "ClassBase.h"

template<typename T>
bool ClassBase::is() const {
    if(dynamic_cast<const T*>(this) != NULL)
        return true;
    return false;
}

template<typename T>
typename add_pointer<typename remove_const<T>::type>::type
    ClassBase::cloneTo() const {

    return clone()->castTo<typename remove_const<T>::type>();
}

template<typename T>
shared_ptr<typename remove_const<T>::type>
    ClassBase::cloneToSharedPtr() const {

    return clone()->castToSharedPtr<typename remove_const<T>::type>();
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
    return dynamic_cast<T* const>(this);
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
    return dynamic_cast<const T* const>(this);
}

template<typename T>
shared_ptr<T> ClassBase::castToSharedPtr() {
    if(!this->is<T>()) {
        cerr << endl << "ERROR @ ClassBase::castTo():"
        << "Invalid cast" << endl;
        assert(false);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return dynamic_pointer_cast<T>(getSharedPtr());
}

template<typename T>
shared_ptr<const T> ClassBase::castToSharedPtr() const {
    if(!this->is<T>()) {
        cerr << endl << "ERROR @ ClassBase::castTo():"
        << "Invalid cast" << endl;
        assert(false);
        exit(EXIT_FAILURE);
        return NULL;
    }
    return dynamic_pointer_cast<const T>(getSharedPtr());
}
