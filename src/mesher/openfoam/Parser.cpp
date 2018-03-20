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

#include "Parser.h"

#include "geometry/element/Triangle3.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Polygon.h"

namespace SEMBA {
namespace Mesher {
namespace OpenFOAM {

Parser::Parser() {
}

Parser::Parser(
        const std::string& dirOpenFoam) {
    dirPolymesh_ = dirOpenFoam + "constant/polyMesh/";
}

Parser::~Parser() {
}

Geometry::Mesh::Unstructured Parser::readMeshUnstructured() const {
    Geometry::CoordR3Group cG = readCoordinates();

    std::vector<FaceIdentifier> face = readFaces();
    std::vector<Geometry::ElemR*> pol(face.size());
    for (std::size_t i = 0; i < face.size(); i++) {
        std::vector<const Geometry::CoordR3*> coords(face[i].second.size());
        for (std::size_t j = 0; j < face[i].second.size(); j++) {
            coords[j] = cG.getId(face[i].second[j]);
        }
        if (face[i].second.size() <= 2) {
            std::cerr << "ERROR @ Parser: "
                    << "Surfaces can not be defined with 2 coords: "
                    << face[i].first << std::endl;
        } else if (face[i].second.size() == 3) {
            pol[i] = new Geometry::Tri3(face[i].first, coords.data());
        } else if (face[i].second.size() == 4) {
            pol[i] = new Geometry::QuaR4(face[i].first, coords.data());
        } else {
            pol[i] = new Geometry::Element::Polygon(face[i].first, coords);
        }
    }
    Geometry::ElemRGroup elems(pol);

    std::vector<Boundary> boundaries = readBoundaries();
    Geometry::Layer::Group<> layers = assignAsLayers(elems, boundaries);
    elems.removeMatId(MatId(0));

    return Geometry::Mesh::Unstructured(cG, elems, layers);
}

void Parser::printInfo() const {
    std::cout << "--- Parser OpenfoamMesh info ---" << std::endl
              << "--- End of Parser Openfoam mesh info ---" << std::endl;
}

Geometry::CoordR3Group Parser::readCoordinates() const {
    std::ifstream file;
    openFile(file, "points");
    if (!file.is_open()) {
        std::cerr << std::endl
                  << "ERROR @ Parser: Could not open file." << std::endl;
        return Geometry::CoordR3Group();
    }

    skipHeader(file);
    std::size_t nCoord = 0;
    while (!file.eof() && nCoord == 0) {
        file >> nCoord;
    }
    std::string line;
    getline(file, line);
    getline(file, line);
    std::vector<Geometry::CoordR3*> coord;
    coord.reserve(nCoord);
    Geometry::CoordId id(0);
    while (!file.eof() && id < Geometry::CoordId(nCoord)) {
        getline(file, line);
        size_t init = line.find("(") + 1;
        size_t end = line.find(")");
        std::string aux = line.substr(init, end-init);
        Math::CVecR3 pos = strToCartesianVector(aux);
        coord.push_back(new Geometry::CoordR3(++id, pos));
    }

    file.close();
    return Geometry::CoordR3Group(coord);
}

Geometry::ElemRGroup* Parser::readSurfaceElements(
        const Geometry::CoordR3Group& cG) const {
    std::vector<FaceIdentifier> face = readFaces();
    const std::size_t nElem = face.size();
    std::vector<Geometry::ElemR*> elems;
    elems.reserve(nElem);
    for (std::size_t i = 0; i < nElem; i++) {
        const Geometry::ElemId id = face[i].first;
        const std::vector<Geometry::CoordId> vId = face[i].second;
        const std::size_t nVertex = vId.size();
        std::vector<const Geometry::CoordR3*> coords(nVertex);
        for (std::size_t j = 0; j < nVertex; j++) {
            coords[j] = cG.getId(vId[j]);
        }
        switch (nVertex) {
        case 3:
            elems.push_back(new Geometry::Tri3(id, coords.data()));
            break;
        case 4:
            elems.push_back(new Geometry::QuaR4(id, coords.data()));
            break;
        default:
            std::cerr << std::endl << "ERROR @ ParserOpenFoamMesh: ";
            std::cerr << "Faces with " << nVertex
                      << "are not supported." << std::endl;
            break;
        }
    }
    return new Geometry::ElemRGroup(elems);
}

std::vector<Boundary> Parser::readBoundaries() const {
    // -- Opens file --
    std::ifstream file;
    openFile(file, "boundary");
    std::vector<Boundary> res;
    if (!file.is_open()) {
        return res;
    }
    // -- Reads data --
    skipHeader(file);
    std::size_t nBoundaries = 0;
    while (!file.eof() && nBoundaries == 0) {
        file >> nBoundaries;
    }
    std::string line;
    getline(file, line);
    getline(file, line);
    //
    std::size_t boundariesRead = 0;
    while (!file.eof() && boundariesRead < nBoundaries) {
        std::size_t nFaces = 0;
        std::size_t startFace = 0;
        std::string name;
        getline(file, name);
        name = trim(name);
        getline(file, line);
        if (line.find("{") != line.npos) {
            bool boundaryFinished = false;
            while (!file.eof() && !boundaryFinished) {
                getline(file, line);
                std::istringstream iss(line);
                std::string tag;
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
        res.push_back(Boundary(name, nFaces, startFace));
        boundariesRead++;
    }
    //
    file.close();
    return res;
}

std::vector<FaceIdentifier> Parser::readFaces() const {
    std::vector<FaceIdentifier> res;
    // --- Opens file ---
    std::ifstream file;
    openFile(file, "faces");
    if (!file.is_open()) {
        return res;
    }
    // --- Reads data ---
    skipHeader(file);
    std::size_t nElem = 0;
    while (!file.eof() && nElem == 0) {
        file >> nElem;
    }
    std::string line;
    getline(file, line);
    getline(file, line);
    Geometry::ElemId id(0);
    res.reserve(nElem);
    while (!file.eof() && id < Geometry::ElemId(nElem)) {
        // Reads info
        getline(file, line);
        size_t init = line.find("(") + 1;
        size_t end = line.find(")");
        const std::size_t nV = atoi(line.substr(0, init).c_str());
        std::vector<Geometry::CoordId> vId(nV);
        std::stringstream ss(line.substr(init, end-init));
        for (std::size_t i = 0; i < nV; i++) {
            ss >> vId[i];
            vId[i]++;
        }
        res.push_back(std::pair<Geometry::ElemId,
                                std::vector<Geometry::CoordId>>(++id, vId));
    }
    file.close();
    return res;
}

std::vector<std::size_t> Parser::readFacesOwner(
        const std::string& ownerOrNeighbour) const {
    std::vector<std::size_t> res;
    // --- Opens file ---
    std::ifstream file;
    openFile(file, ownerOrNeighbour.c_str());
    if (!file.is_open()) {
        return res;
    }
    // --- Reads data ---
    skipHeader(file);
    std::size_t nElem = 0;
    while (!file.eof() && nElem == 0) {
        file >> nElem;
    }
    std::string line;
    getline(file, line);
    getline(file, line);
    std::size_t elem = 0;
    res.reserve(nElem);
    while (!file.eof() && elem < nElem) {
        std::size_t id;
        file >> id;
        res.push_back(id++);
        elem++;
    }
    file.close();
    return res;
}

bool Parser::isExistingDirectory(const std::string& dir) const {
    struct stat sb;
    bool res = (stat(dir.c_str(), &sb) == 0);
#ifndef _WIN32
    res &= S_ISDIR(sb.st_mode);
#endif
    return res;
}

void Parser::openFile(std::ifstream& file,
                      const std::string& name) const {
    // Opens file
    std::string filename = dirPolymesh_ + name;
    file.open(filename.c_str(), std::ifstream::in);
    if (file.fail()) {
        std::cerr << std::endl << "WARNING @ ParserOpenFoamMesh: ";
        std::cerr << std::endl << "Could not open file: ";
        std::cerr << filename << std::endl;
    }
    return;
}

void Parser::skipHeader(std::ifstream& file) const {
    std::string line;
    while (!file.eof()) {
        getline(file, line);
        if (line.find("}") != line.npos) {
            getline(file,line);
            return;
        }
    }
    std::cerr << std::endl << "ERROR @ ParserOpenFoamMesh: "
            << "End of file reached and EOF was not found."
            << "While skipping header. " << std::endl;
    return;
}

Geometry::Layer::Group<> Parser::assignAsLayers(
        Geometry::ElemRGroup& elems,
        const std::vector<Boundary>& bound) const {
    Geometry::Layer::Group<> layers;
    for (std::size_t b = 0; b < bound.size(); b++) {
        if (bound[b].isMaterial()) {
            const Geometry::Layer::Layer* lay =
                layers.addId(new Geometry::Layer::Layer(bound[b]))(0);
            MatId matId = bound[b].getMaterialIdFromName();
            const PhysicalModel::PhysicalModel* mat = nullptr; //TODO
            Geometry::LayerId layId = bound[b].getId();
            for (std::size_t i = 0; i < bound[b].getFaces(); i++) {
                Geometry::ElemId id(bound[b].getStartFace() + i + 1);
                elems.setModel(id, mat);
                elems.setLayer(id, lay);
            }
        }
    }
    return layers;
}

}
}
}
