#include "MeshStructured.h"

template<template<typename> class E>
ElementsGroup< E<Int> > MeshStructured::add(
        const ElementsGroup<E<Real> >& rhs,
        const Real tol) {
    const ElementsGroup<const E<Real> > aux = rhs;
    return add(aux, tol);
}

template<template<typename> class E>
ElementsGroup< E<Int> > MeshStructured::add(
        const ElementsGroup<const E<Real> >& rhs,
        const Real tol) {
    vector<ElementId> elemIds;
    ElemI* elem;
    elemIds.reserve(rhs.size());
    for(UInt i = 0; i < rhs.size(); i++) {
        elem = rhs(i)->toStructured(*this, *this, tol);
        if (elem != NULL) {
            if (elems().existId(elem->getId())) {
                const ElemI* orig = elems().get(elem->getId());
                if (*elem != *orig) {
                    cerr << endl << "ERROR @ MeshStructured::add(): "
                    << "Existent Element not coincident." << endl;
                    assert(false);
                    exit(EXIT_FAILURE);
                }
                elemIds.push_back(elem->getId());
            } else {
                elems().add(elem);
                elemIds.push_back(elem->getId());
            }
        }
    }
    return elems().getGroupWith(elemIds);
}
