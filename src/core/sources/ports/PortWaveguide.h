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
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "Port.h"

class PortWaveguide : public Port {
public:
	typedef enum {
		TE,
		TM
	} ExcitationMode;

	PortWaveguide(Magnitude* magnitude,
             const GroupElements<const Surf>& elem,
	         const ExcitationMode excMode,
	         const pair<UInt,UInt> mode);
	PortWaveguide(const PortWaveguide& rhs);
	virtual ~PortWaveguide();

    bool hasSameProperties(const EMSourceBase& rhs) const;

	ExcitationMode getExcitationMode() const;
	pair<UInt, UInt> getMode() const;

    void printInfo() const;
private:
	ExcitationMode excitationMode_;
	pair<UInt,UInt> mode_;
};

#endif /* WAVEPORT_H_ */
