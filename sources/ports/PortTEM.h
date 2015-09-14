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

    PortTEM();
    virtual ~PortTEM();

    ExcitationMode getExcitationMode() const;
};

#endif /* SRC_COMMON_SOURCES_PORTTEM_H_ */
