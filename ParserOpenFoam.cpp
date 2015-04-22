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

MeshUnstructured
ParserOpenFoam::readMeshUnstructured() const {
    CoordR3Group cG = readCoordinates();

    vector<FaceIdentifier> face = readFaces();
    vector<ElemR*> pol(face.size());
    for (UInt i = 0; i < face.size(); i++) {
        if (face[i].second.size() <= 2) {
            cerr << "ERROR @ ParserOpenfoam: "
                    << "Surfaces can not be defined with 2 coords: "
                    << face[i].first << endl;
        } else if (face[i].second.size() == 3) {
            pol[i] = new Tri3(cG, face[i].first, &face[i].second[0]);
        } else if (face[i].second.size() == 4) {
            pol[i] = new QuaR4(cG, face[i].first, &face[i].second[0]);
        } else {
            pol[i] = new Polygon(cG, face[i].first, face[i].second);
        }
    }
    ElemRGroup elems(pol);

    vector<OpenfoamBoundary> boundaries = readBoundaries();
    GroupLayers<> layers = assignAsLayers(elems, boundaries);
    elems.remove(MatId(0));

    return MeshUnstructured(cG, elems, layers);
}

void ParserOpenFoam::printInfo() const {
    cout<< "--- Parser OpenfoamMesh info ---" << endl
            << "--- End of Parser Openfoam mesh info ---" << endl;
}

CoordR3Group ParserOpenFoam::readCoordinates() const {
    ifstream file;
    openFile(file, "points");
    if (!file.is_open()) {
        cerr << endl << "ERROR @ ParserOpenfoam: Could not open file." << endl;
        return CoordR3Group();
    }

    skipHeader(file);
    UInt nCoord = 0;
    while (!file.eof() && nCoord == 0) {
        file >> nCoord;
    }
    string line;
    getline(file, line);
    getline(file, line);
    vector<CoordR3*> coord;
    coord.reserve(nCoord);
    CoordinateId id(0);
    while (!file.eof() && id < nCoord) {
        getline(file, line);
        size_t init = line.find("(") + 1;
        size_t end = line.find(")");
        CVecR3 pos = strToCartesianVector(line.substr(init, end-init));
        coord.push_back(new CoordR3(++id, pos));
    }

    file.close();
    return CoordR3Group(coord);
}

ElemRGroup* ParserOpenFoam::readSurfaceElements(
        const CoordR3Group& cG) const {
    vector<FaceIdentifier> face = readFaces();
    const UInt nElem = face.size();
    vector<ElemR*> elems;
    elems.reserve(nElem);
    for (UInt i = 0; i < nElem; i++) {
        const ElementId id = face[i].first;
        const vector<CoordinateId> vId = face[i].second;
        const UInt nVertex = vId.size();
        switch (nVertex) {
        case 3:
            elems.push_back(new Tri3(cG, id, &vId.front()));
            break;
        case 4:
            elems.push_back(new QuaR4(cG, id, &vId.front()));
            break;
        default:
            cerr << endl << "ERROR @ ParserOpenFoamMesh: ";
            cerr << endl << "Faces with " << nVertex << "are not supported." << endl;
            break;
        }
    }
    return new ElemRGroup(elems);
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
    UInt nBoundaries = 0;
    while (!file.eof() && nBoundaries == 0) {
        file >> nBoundaries;
    }
    string line;
    getline(file, line);
    getline(file, line);
    //
    UInt boundariesRead = 0;
    while (!file.eof() && boundariesRead < nBoundaries) {
        UInt nFaces = 0;
        UInt startFace = 0;
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
        res.push_back(OpenfoamBoundary(name, nFaces, startFace));
        boundariesRead++;
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
    UInt nElem = 0;
    while (!file.eof() && nElem == 0) {
        file >> nElem;
    }
    string line;
    getline(file, line);
    getline(file, line);
    ElementId id(0);
    res.reserve(nElem);
    while (!file.eof() && id < nElem) {
        // Reads info
        getline(file, line);
        size_t init = line.find("(") + 1;
        size_t end = line.find(")");
        const UInt nV = atoi(line.substr(0, init).c_str());
        vector<CoordinateId> vId(nV);
        stringstream ss(line.substr(init, end-init));
        for (UInt i = 0; i < nV; i++) {
            ss >> vId[i];
            vId[i]++;
        }
        res.push_back(pair<ElementId,vector<CoordinateId> >(++id, vId));
    }
    file.close();
    return res;
}

vector<UInt>
ParserOpenFoam::readFacesOwner(
        const string& ownerOrNeighbour) const {
    vector<UInt> res;
    // --- Opens file ---
    ifstream file;
    openFile(file, ownerOrNeighbour.c_str());
    if (!file.is_open()) {
        return res;
    }
    // --- Reads data ---
    skipHeader(file);
    UInt nElem = 0;
    while (!file.eof() && nElem == 0) {
        file >> nElem;
    }
    string line;
    getline(file, line);
    getline(file, line);
    UInt elem = 0;
    res.reserve(nElem);
    while (!file.eof() && elem < nElem) {
        UInt id;
        file >> id;
        res.push_back(id++);
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
        cerr << endl << "WARNING @ ParserOpenFoamMesh: ";
        cerr << endl << "Could not open file: ";
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
    cerr << endl << "ERROR @ ParserOpenFoamMesh: "
            << "End of file reached and EOF was not found."
            << "While skipping header. " << endl;
    return;
}

GroupLayers<> ParserOpenFoam::assignAsLayers(
        ElemRGroup& elems,
        const vector<OpenfoamBoundary>& bound) const {
    GroupLayers<> layers;
    for (UInt b = 0; b < bound.size(); b++) {
        if (bound[b].isMaterial()) {
            layers.add(new Layer(bound[b]), true);
            MatId matId = bound[b].getMaterialIdFromName();
            LayerId layId = bound[b].getId();
            for (UInt i = 0; i < bound[b].getFaces(); i++) {
                ElementId id(bound[b].getStartFace() + i + 1);
                elems.setMatId(id, matId);
                elems.setLayerId(id, layId);
            }
        }
    }
    return layers;
}
