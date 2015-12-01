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
 * PMMultiportPredefined.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMMULTIPORTPREDEFINED_H_
#define PMMULTIPORTPREDEFINED_H_

#include "PMMultiport.h"

class PMMultiportPredefined: public PMMultiport {
public:
	PMMultiportPredefined(
	 const MatId idIn,
	 const string nameIn,
	 const PMMultiport::Type);
	virtual ~PMMultiportPredefined();

    DEFINE_CLONE(PMMultiportPredefined);

	virtual void printInfo() const;
};

#endif /* PMMULTIPORTPREDEFINED_H_ */
