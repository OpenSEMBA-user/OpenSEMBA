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
bool OutRq<T>::isSimilar(const T2* rhs, const bool rev) const {
    if (!rhs->is<OutRq<T> >() || !this->is<T2>()) {
        return false;
    }
    bool isSimilar = true;
    isSimilar &= getName() == rhs->getName();
    isSimilar &= getOutputType() == rhs->getOutputType();
    isSimilar &= Domain::operator==(*rhs);
    if (!rev) {
        isSimilar &= rhs->isSimilar(this->castTo<T2>(), true);
    }
    return isSimilar;
}

template<class T>
inline void OutRq<T>::printInfo() const {
    cout << " --- OutRq Info --- " << endl;
    OutRq<void>::printInfo();
    ElementsGroup<T>::printInfo();
}
