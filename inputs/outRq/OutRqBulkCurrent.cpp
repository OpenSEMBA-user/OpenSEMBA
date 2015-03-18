/*
 * OutRqBulkCurrent.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#include "../outRq/OutRqBulkCurrent.h"

OutRqBulkCurrent::OutRqBulkCurrent(
        const ElementsGroup<Surface<> >& elems,
        const Domain& domain,
        const OutRq::Type& type,
        const string& name) :
        OutRq(domain, type, name), ElementsGroup<Surface<> >(elems){
}
