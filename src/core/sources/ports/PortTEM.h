// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
