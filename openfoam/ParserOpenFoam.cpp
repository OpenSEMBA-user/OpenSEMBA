/*
 * ParserOpenFoamMesh.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#include "ParserOpenFoam.h"

ParserOpenFoam::ParserOpenFoam() {
}

ParserOpenFoam::ParserOpenFoam(
 const string& dirOpenFoam) {
	dirPolymesh_ = dirOpenFoam + "constant/polyMesh/";
}

ParserOpenFoam::~ParserOpenFoam() {
}

MeshOpenfoam
ParserOpenFoam::readMeshOpenfoam() const {
	CoordinateGroup cG = readCoordinates();
	vector<FaceIdentifier> faces = readFaces();
	vector<uint> faceOwner = readFacesOwner(string("owner"));
	vector<uint> faceNeighbour = readFacesOwner(string("neighbour"));
	vector<OpenfoamBoundary> boundaries = readBoundaries();
	MeshOpenfoam res(cG, faces, faceOwner, faceNeighbour, boundaries);
	return res;
}

void
ParserOpenFoam::printInfo() const {
	cout<< "--- Parser OpenfoamMesh info ---" << endl
		<< "--- End of Parser Openfoam mesh info ---" << endl;
}

CoordinateGroup
ParserOpenFoam::readCoordinates() const {
	// Opens file
	ifstream file;
	openFile(file, "points");
	if (!file.is_open()) {
		return CoordinateGroup();
	}
	// Reads data.
	skipHeader(file);
	uint nCoord = 0;
	while (!file.eof() && nCoord == 0) {
		file >> nCoord;
	}
	string line;
	getline(file, line);
	getline(file, line);
	vector<Coordinate<double,3> > coord;
	coord.reserve(nCoord);
	uint id = 0;
	while (!file.eof() && id < nCoord) {
		getline(file, line);
		size_t init = line.find("(") + 1;
		size_t end = line.find(")");
		CVecD3 pos = strToCartesianVector(line.substr(init, end-init));
		coord.push_back(Coordinate<double,3>(id++, pos));
	}
	// Closes file.
	file.close();
	return CoordinateGroup(coord);
}

ElementsGroup
ParserOpenFoam::readSurfaceElements(
 const CoordinateGroup& cG) const {
	vector<FaceIdentifier> face = readFaces();
	const uint nElem = face.size();
	vector<Tri3> tri3;
	vector<Quad4> quad4;
	tri3.reserve(nElem);
	quad4.reserve(nElem);
	uint matId = 0;
	CVecD3 normal;
	for (uint i = 0; i < nElem; i++) {
		const uint id = face[i].first;
		const vector<uint> vId = face[i].second;
		const uint nVertex = vId.size();
		switch (nVertex) {
		case 3:
			tri3.push_back(Tri3(cG, id, matId, &vId.front(), normal));
			break;
		case 4:
			quad4.push_back(Quad4(cG, id, matId, &vId.front()));
			break;
		default:
			cerr << "ERROR @ ParserOpenFoamMesh: ";
			cerr << "Faces with " << nVertex << "are not supported." << endl;
			break;
		}
	}
	return ElementsGroup(tri3, quad4);
}

vector<OpenfoamBoundary>
ParserOpenFoam::readBoundaries() const {
	// -- Opens file --
	ifstream file;
	openFile(file, "boundary");
	vector<OpenfoamBoundary> res;
	if (!file.is_open()) {
		return res;
	}
	// -- Reads data --
	skipHeader(file);
	uint nBoundaries = 0;
	while (!file.eof() && nBoundaries == 0) {
		file >> nBoundaries;
	}
	string line;
	getline(file, line);
	getline(file, line);
	//
	uint id = 0;
	while (!file.eof() && id < nBoundaries) {
		uint nFaces = 0;
		uint startFace = 0;
		string name;
		getline(file, name);
		name = trim(name);
		getline(file, line);
		if (line.find("{") != line.npos) {
			bool boundaryFinished = false;
			while (!file.eof() && !boundaryFinished) {
				getline(file, line);
				istringstream iss(line);
				string tag;
				iss >> tag;
				if (tag == "nFaces") {
					iss >> nFaces;
				} else if (tag == "startFace") {
					iss >> startFace;
				} else if (tag == "}") {
					boundaryFinished = true;
				}
			}
		}
		res.push_back(OpenfoamBoundary(id++, name, nFaces, startFace));
	}
	//
	file.close();
	return res;
}


vector<FaceIdentifier>
ParserOpenFoam::readFaces() const {
	vector<FaceIdentifier> res;
	// --- Opens file ---
	ifstream file;
	openFile(file, "faces");
	if (!file.is_open()) {
		return res;
	}
	// --- Reads data ---
	skipHeader(file);
	uint nElem = 0;
	while (!file.eof() && nElem == 0) {
		file >> nElem;
	}
	string line;
	getline(file, line);
	getline(file, line);
	uint id = 0;
	res.reserve(nElem);
	while (!file.eof() && id < nElem) {
		// Reads info
		getline(file, line);
		size_t init = line.find("(") + 1;
		size_t end = line.find(")");
		const uint nV = atoi(line.substr(0, init).c_str());
		vector<uint> vId(nV);
		stringstream ss(line.substr(init, end-init));
		for (uint i = 0; i < nV; i++) {
			ss >> vId[i];
		}
		res.push_back(pair<uint,vector<uint> >(id++, vId));
	}
	file.close();
	return res;
}

vector<uint>
ParserOpenFoam::readFacesOwner(
 const string& ownerOrNeighbour) const {
	vector<uint> res;
	// --- Opens file ---
	ifstream file;
	openFile(file, ownerOrNeighbour.c_str());
	if (!file.is_open()) {
		return res;
	}
	// --- Reads data ---
	skipHeader(file);
	uint nElem = 0;
	while (!file.eof() && nElem == 0) {
		file >> nElem;
	}
	string line;
	getline(file, line);
	getline(file, line);
	uint elem = 0;
	res.reserve(nElem);
	while (!file.eof() && elem < nElem) {
		uint id;
		file >> id;
		res.push_back(id);
		elem++;
	}
	file.close();
	return res;
}

bool
ParserOpenFoam::isExistingDirectory(const string& dir) const {
	struct stat sb;
	bool res = (stat(dir.c_str(), &sb) == 0);
	res &= S_ISDIR(sb.st_mode);
	return res;
}

void
ParserOpenFoam::openFile(
 ifstream& file,
 const string& name) const {
	// Opens file
	string filename = dirPolymesh_ + name;
	file.open(filename.c_str(), ifstream::in);
	if (file.fail()) {
		cerr << "WARNING @ ParserOpenFoamMesh: ";
		cerr << "Could not open file: ";
		cerr << filename << endl;
	}
	return;
}

void
ParserOpenFoam::skipHeader(ifstream& file) const {
	string line;
	while (!file.eof()) {
		getline(file, line);
		if (line.find("}") != line.npos) {
			getline(file,line);
			return;
		}
	}
	cerr << "ERROR @ ParserOpenFoamMesh: ";
	cerr << "End of file reached and EOF was not found.";
	cerr << "While skipping header. " << endl;
	return;
}
