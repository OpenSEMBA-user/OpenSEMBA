#include "OutRq.h"

template<class T>
OutRq<T>::OutRq(const Domain& domain,
                const Type outputType,
                const string& name,
                const ElementsGroup<T>& elems)
:   Domain(domain),
    OutRq<void>(outputType, name),
    ElementsGroup<T>(elems) {

}

template<class T>
OutRq<T>::~OutRq() {

}

template<class T>
ClassBase* OutRq<T>::clone() const {
    return new OutRq<T>(*this);
}

template<class T>
OutRq<T>::OutRq(const OutRq<T>& rhs)
:   Domain(rhs),
    OutRq<void>(rhs),
    ElementsGroup<T>(rhs) {

}

template <class T> template<class T2>
bool OutRq<T>::isSimilar(const OutRq<T2>* rhs, const bool rev) const {
    bool isSimilar = true;
    isSimilar &= getName() == rhs->getName();
    isSimilar &= getOutputType() == rhs->getOutputType();
    isSimilar &= Domain::operator==(*rhs);
    if (!rev) {
        isSimilar &= rhs->isSimilar(rhs, true);
    }
    return isSimilar;
}

template<class T> template<class T2>
void OutRq<T>::add(OutRq<T2>* rhs) {
    if (this->isSimilar(rhs)) {
        this->add(rhs);
    }
}
