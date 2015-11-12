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
 * ParserOpenFoamMesh.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef PARSEROPENFOAMMESH_H_
#define PARSEROPENFOAMMESH_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>

#include "OpenfoamBoundary.h"

#include "parser/Parser.h"

typedef pair<ElementId,vector<CoordinateId> > FaceIdentifier;

class ParserOpenFoam : public Parser {
public:
	ParserOpenFoam();
	ParserOpenFoam(const string& openfoamDir);
	virtual ~ParserOpenFoam();
	MeshUnstructured readMeshUnstructured() const;
	bool isExistingDirectory(const string& dir) const;
	void printInfo() const;
private:
	string dirPolymesh_;
	CoordR3Group readCoordinates() const;
	void skipHeader(ifstream& file) const;
	ElemRGroup* readSurfaceElements(const CoordR3Group&) const;
	vector<FaceIdentifier> readFaces() const;
	vector<UInt> readFacesOwner(const string& ownerOrNeighbour) const;
	vector<OpenfoamBoundary> readBoundaries() const;
	GroupLayers<> assignAsLayers(
	        ElemRGroup& elems,
	        const vector<OpenfoamBoundary>& boundaries) const;
	void openFile(ifstream& file, const string& name) const;
};

#endif /* PARSEROPENFOAMMESH_H_ */
