#include "MeshStructured.h"

template<template<typename> class E>
ElementsGroup< E<Int> > MeshStructured::add(const ElementsGroup< E<Real> >& rhs) {
    vector< E<Int>* > elems;
    ElemI* elem;
    elems.reserve(rhs.size());
    for(UInt i = 0; i < rhs.size(); i++) {
        elem = rhs(i)->toStructured(*this, *this);
        if (elem != NULL) {
            if (ElementsGroup<ElemI>::existId(elem->getId())) {
                ElemI* orig =
                ElementsGroup<ElemI>::element_[
                                               ElementsGroup<ElemI>::mapId_[elem->getId()]];
                if (*elem != *orig) {
                    cerr << endl << "ERROR @ MeshStructured::add(): "
                    << "Existent Element not coincident." << endl;
                    assert(false);
                    exit(EXIT_FAILURE);
                }
                elems.push_back(orig->castTo< E<Int> >());
            } else {
                ElementsGroup<ElemI>::add(elem);
                elems.push_back(elem->castTo< E<Int> >());
            }
        }
    }
    return ElementsGroup< E<Int> >(elems, false);
}
