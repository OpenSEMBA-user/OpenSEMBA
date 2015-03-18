/*
 * OutRqPoint.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#include "../outRq/OutRqPoint.h"

OutRqPoint::OutRqPoint(
        const ElementsGroup<Node<> >& elems,
        const Domain& domain,
        const Type& type,
        const string& name) :
        OutRq(domain, type, name), ElementsGroup<Node<> >(elems) {
}

OutRqPoint::~OutRqPoint() {
    // TODO Auto-generated destructor stub
}


ClassBase* OutRqPoint::clone() const {
   return new OutRqPoint(*this);
}
