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
 * PMVolumePML.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMVOLUMEPML_H_
#define PMVOLUMEPML_H_

#include "PMVolume.h"
#include "math/LocalAxes.h"

class PMVolumePML : public PMVolume {
public:
    PMVolumePML(
	        const MatId id,
	        const string& name,
	        const LocalAxes* orientation = NULL);
    // If no orientation is given. Automatic orientation should be attempted.

	~PMVolumePML();

	DEFINE_CLONE(PMVolumePML);

	const LocalAxes* getOrientation() const;

	void printInfo() const;
private:
	const LocalAxes* orientation_;
};

#endif /* PMVOLUMEPML_H_ */
