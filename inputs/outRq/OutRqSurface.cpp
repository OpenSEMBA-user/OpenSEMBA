/*
 * OutRqBulkCurrent.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#include "OutRqSurface.h"

OutRqSurface::OutRqSurface(
        const ElementsGroup<Surface<> >& elems,
        const Domain& domain,
        const OutRq::Type& type,
        const string& name) :
        OutRq(domain, type, name), ElementsGroup<Surface<> >(elems){
}

ClassBase* OutRqSurface::clone() const {
   return new OutRqSurface(*this);
}
