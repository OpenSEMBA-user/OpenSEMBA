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


class PMVolumePML : public PMVolume {
public:
	typedef enum {
		plus = 0,
		minus = 1,
		none = 2
	} Direction;
	enum class Orientation {
		PMLx,
		PMLy,
		PMLz,
		PMLxy,
		PMLyz,
		PMLzx,
		PMLxyz,
		undefined
	} ;
	static const UInt possibleDirections = 26;
	PMVolumePML(const MatId id, const string& name_);
	PMVolumePML(
	 const MatId id,
	 const Direction direction[3],
	 const BoxR3& bound);
	~PMVolumePML();

	DEFINE_CLONE(PMVolumePML);

	const Direction* getDirection() const;
	Orientation getOrientation() const;
	bool isUniaxial() const;
	bool isBiaxial() const;
	bool isTriaxial() const;
	const pair<CVecR3, CVecR3>& getBound() const;
	CVecR3 getWidth() const;
	UInt getFirstOrientationIndex() const;
	CVecR3 getPMLBeginningPosition() const;
	void printInfo() const;
private:
	Direction direction[3];
	pair<CVecR3,CVecR3> bound;
};

#endif /* PMVOLUMEPML_H_ */
