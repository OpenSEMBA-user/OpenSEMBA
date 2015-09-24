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
 * ProblemSize.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#ifndef PROBLEMSIZE_H_
#define PROBLEMSIZE_H_

#include <iostream>

using namespace std;

#include "Types.h"

class ProblemSize {
public:
	UInt v;	    // Number of coordinate points.
	UInt lin2;  // # linear segment lines.
	UInt lin3;  // # quadratic segment lines.
	UInt tri3;  // # triangular elements.
	UInt tri6;  // #
	UInt tet4;  // # tetrahedron elements.
	UInt tet10; // #
	UInt hex8;
	UInt mat;   // # materials.
	ProblemSize();
	ProblemSize
	 operator=(const ProblemSize& rhs);
	void
     printInfo() const;
};

#endif /* PROBLEMSIZE_H_ */
