/*
 * OutRqPoint.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#include "OutRqNode.h"

OutRqNode::OutRqNode(
        const ElementsGroup<Node<> >& elems,
        const Domain& domain,
        const Type& type,
        const string& name) :
        OutRq(domain, type, name), ElementsGroup<Node<> >(elems) {
}

ClassBase* OutRqNode::clone() const {
   return new OutRqNode(*this);
}
