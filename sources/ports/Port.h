/*
 * Port.h
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORT_H_
#define SRC_COMMON_SOURCES_PORT_H_

#include "../EMSource.h"
#include "../../options/OptionsMesher.h"
#include "base/error/ErrorNotImplemented.h"

class Port : public EMSource<Surf> {
public:

    Port(Magnitude* magnitude,
            const GroupElements<const Surf>& elem);
    Port(const Port& rhs);
    virtual ~Port();

    CVecR3 getNormal() const;

    virtual CVecR3 getOrigin(const BoundTerminations& sym) const = 0;
    virtual CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym) const = 0;
};

#endif /* SRC_COMMON_SOURCES_PORT_H_ */
