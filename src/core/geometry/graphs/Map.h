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
#ifndef MAP_H_
#define MAP_H_

#include "../elements/GroupElements.h"
#ifdef USE_OPENMP
#include <omp.h>
#endif

using namespace std;

#define NUMBER_OF_VERTEX_LIMIT 2.5E6 // Max. number of vertices.
//#define MAP_DO_NOT_BUILD_SURFACE_MAPS

class Map {
public:
   Map();
   Map(const ElemR* elem);
	virtual ~Map();
	virtual UInt
	 getVolToF(UInt f) const = 0;
   virtual UInt
    getLocalId() const = 0;
   virtual void
    reassignPointers(const GroupElements<const Elem>& nEG) = 0;
	virtual void
	 printInfo() const = 0;
};

#endif
