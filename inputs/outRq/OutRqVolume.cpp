/*
 * OutRqVolume.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#include "../outRq/OutRqVolume.h"

OutRqVolume::OutRqVolume(
        const ElementsGroup<Volume<> >& box,
        const Domain& domain,
        const Type& type,
        const string& name) :
        OutRq(domain, type, name), ElementsGroup<Volume<> >(box) {
}
