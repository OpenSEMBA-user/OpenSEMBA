#include "OutRq.h"

template<class T>
OutRq<T>::OutRq(const Domain& domain,
                const Type outputType,
                const string& name,
                const ElementsGroup<T>& elems)
:   Domain(domain),
    OutRqBase(outputType, name),
    ElementsGroup<const T>(elems) {

}

template<class T>
OutRq<T>::OutRq(const OutRq<T>& rhs)
:   Domain(rhs),
    OutRqBase(rhs),
    ElementsGroup<const T>(rhs) {

}

template<class T>
OutRq<T>::~OutRq() {

}

template <class T>
bool OutRq<T>::isSimilar(const OutRqBase& rhs) const {
    if (!OutRqBase::hasSameProperties(rhs)) {
        return false;
    }
    const OutRq<T>* rhsPtr = rhs.template castTo<OutRq<T> >();
    bool isSimilar = true;
    isSimilar &= getName() == rhsPtr->getName();
    isSimilar &= getOutputType() == rhsPtr->getOutputType();
    isSimilar &= Domain::operator==(*rhsPtr);
    return isSimilar;
}

template <class T>
void OutRq<T>::set(const ElementsGroup<const Elem>& elems) {
    ElementsGroup<const T>::operator=(elems);
}

template <class T>
void OutRq<T>::add(const ElementsGroup<const Elem>& elems) {
    ElementsGroup<const T>::add(elems);
}

template<class T>
inline void OutRq<T>::printInfo() const {
    cout << " --- Output request instance --- " << endl;
    OutRqBase::printInfo();
    ElementsGroup<const T>::printInfo();
}
