#include "OutRq.h"

template<class T>
OutRq<T>::OutRq(const Domain& domain,
                const Type outputType,
                const string& name,
                const GroupElements<T>& elems)
:   Domain(domain),
    OutRqBase(outputType, name),
    GroupElements<const T>(elems) {

}

template<class T>
OutRq<T>::OutRq(const OutRq<T>& rhs)
:   Domain(rhs),
    OutRqBase(rhs),
    GroupElements<const T>(rhs) {

}

template<class T>
OutRq<T>::~OutRq() {

}

template<class T>
bool OutRq<T>::isSimilar(const OutRqBase& rhs) const {
    if (!this->hasSameProperties(rhs)) {
        return false;
    }
    UInt sizeLhs = this->elems().size();
    UInt sizeRhs = rhs.elems().size();
    if ((sizeLhs == 0) && (sizeRhs == 0)) {
        return true;
    } else if ((sizeLhs == 0) || (sizeRhs == 0)) {
        return false;
    }
    return true;
}

template <class T>
void OutRq<T>::set(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::operator=(elems);
}

template <class T>
void OutRq<T>::add(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::add(elems);
}

template<class T>
inline void OutRq<T>::printInfo() const {
    cout << " --- Output request instance --- " << endl;
    OutRqBase::printInfo();
    GroupElements<const T>::printInfo();
}
