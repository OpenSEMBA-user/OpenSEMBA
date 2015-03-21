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
OutRq<T>::OutRq(const OutRq<T>& rhs)
:   Domain(rhs),
    OutRq<void>(rhs),
    ElementsGroup<T>(rhs) {

}

template<class T>
OutRq<T>::~OutRq() {

}

template<class T>
ClassBase* OutRq<T>::clone() const {
    return new OutRq<T>(*this);
}

template<class T>
bool OutRq<T>::isSimilar(const OutRq<void>& rhs, const bool rev) const {
    if(!rhs.template is<OutRq<T> >()) {
        return false;
    }
    bool isSimilar = OutRq<void>::isSimilar(rhs, true);
    if (!rev) {
        isSimilar &= rhs.isSimilar(rhs, true);
    }
    return isSimilar;
}
