/*
 * PortTEM.h
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORTTEM_H_
#define SRC_COMMON_SOURCES_PORTTEM_H_

#include "Port.h"

class PortTEM : public Port {
public:
    typedef enum {
        voltage,
        current
    } ExcitationMode;

    PortTEM(Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excitationMode);
    PortTEM(const PortTEM& rhs);
    virtual ~PortTEM();

    ExcitationMode getExcitationMode() const;

    void printInfo() const;

    static string toStr(const ExcitationMode& excitationMode);
private:
    ExcitationMode excitationMode_;
};

#endif /* SRC_COMMON_SOURCES_PORTTEM_H_ */
