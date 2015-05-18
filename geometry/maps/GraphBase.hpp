/*
 * GraphBase.hpp
 *
 *  Created on: 17/5/2015
 *      Author: Daniel
 */

#include "GraphBase.h"

template<class ELEM, class BOUND>
GraphBase<ELEM,BOUND>::GraphBase() {

}

template<class ELEM, class BOUND>
GraphBase<ELEM,BOUND>::~GraphBase() {
    for (UInt i = 0; i < bounds_.size(); i++) {
        delete bounds_[i];
    }
    for (UInt i = 0; i < elems_.size(); i++) {
        delete elems_[i];
    }
}

template<class ELEM, class BOUND>
void GraphBase<ELEM,BOUND>::printInfo() const {
    cout << "--- Graph Info ---" << endl;
    cout << "Elems:" << endl;
    for (UInt i = 0; i < elems_.size(); i++) {
        elems_[i]->printInfo();
    }
    cout << "Bounds:" << endl;
    for (UInt i = 0; i < bounds_.size(); i++) {
        bounds_[i]->printInfo();
    }
}
