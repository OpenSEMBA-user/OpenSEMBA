#include "MeshStructured.h"

template<template<typename> class E>
ElementsGroup< E<Int> > MeshStructured::add(
        const ElementsGroup< E<Real> >& rhs,
        const Real tol) {
    vector<ElementId> elemIds;
    ElemI* elem;
    elemIds.reserve(rhs.size());
    for(UInt i = 0; i < rhs.size(); i++) {
        elem = rhs(i)->toStructured(*this, *this, tol);
        if (elem != NULL) {
            if (elems().existId(elem->getId())) {
                const ElemI* orig = elems().getPtrToId(elem->getId());
                if (*elem != *orig) {
                    elems().add(elem, true);
                }
                elemIds.push_back(elem->getId());
            } else {
                elems().add(elem);
                elemIds.push_back(elem->getId());
            }
        }
    }
    return elems().get(elemIds);
}

template<template<typename> class E>
ElementsGroup< E<Int> > MeshStructured::add(
        E<Real>* rhs,
        const Real tol) {
    ElementsGroup<E<Real> > aux;
    aux.add(rhs, true);
    return this->add(aux, tol);
}
