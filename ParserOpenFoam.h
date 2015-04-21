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
	LayerGroup<> assignAsLayers(
	        ElemRGroup& elems,
	        const vector<OpenfoamBoundary>& boundaries) const;
	void openFile(ifstream& file, const string& name) const;
};

#endif /* PARSEROPENFOAMMESH_H_ */
