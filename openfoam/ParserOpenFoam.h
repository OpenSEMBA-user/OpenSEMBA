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
#include "MeshOpenfoam.h"
#include "../parser/Parser.h"
#include "OpenfoamBoundary.h"

class ParserOpenFoam : public Parser {
public:
	ParserOpenFoam();
	ParserOpenFoam(
	 const string& openfoamDir);
	virtual ~ParserOpenFoam();
	virtual MeshOpenfoam
	 readMeshOpenfoam() const;
	virtual bool
	 isExistingDirectory(const string& dir) const;
	virtual void
	 printInfo() const;
private:
	string dirPolymesh_;
	CoordinateGroup
	 readCoordinates() const;
	void
	 skipHeader(ifstream& file) const;
	ElementsGroup
	 readSurfaceElements(const CoordinateGroup&) const;
	vector<FaceIdentifier>
	 readFaces() const;
	vector<uint>
	 readFacesOwner(
	  const string& ownerOrNeighbour) const;
	vector<OpenfoamBoundary>
	 readBoundaries() const;
	void
	 openFile(
	  ifstream& file,
	  const string& name) const;
};

#endif /* PARSEROPENFOAMMESH_H_ */
