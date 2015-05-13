/*
 * ParserSTL.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#include "ParserSTL.h"

ParserSTL::ParserSTL() {
}

ParserSTL::ParserSTL(const string& fn)
:   ProjectFile(fn) {
}

ParserSTL::~ParserSTL() {

}

SmbData*
ParserSTL::read() {

    // Reads coordinates.

    ifstream stl;
    openAsInput(stl);
    string label;
    vector<CVecR3> vertices;
    UInt nLines = count(istreambuf_iterator<char>(stl),
            istreambuf_iterator<char>(), '\n');
    vertices.reserve(nLines);
    stl.seekg(0); // Rewinds.
    while (!stl.eof()) {
        stl >> label;
        if (label == "vertex") {
            CVecR3 vertex;
            stl >> vertex(x) >> vertex(y) >> vertex(z);
            vertices.push_back(vertex);
        }
    }
    GroupCoordinates<CoordR3> cG;
    cG.add(vertices);
    stl.close();

    // Reads Elements and Layers.
    GroupLayers<Layer> lG;
    GroupElements<ElemR> eG;
    openAsInput(stl);
    while (!stl.eof()) {
        stl >> label;
        if (label == "solid") {
            string layerName;
            stl >> layerName;
            Layer* lay = lG.add(new Layer(layerName), true);
            LayerId lId = lay->getId();
            string line;
            while (!stl.eof()) {
                getline(stl, line);
                if (trim(line) == "outer loop") {
                    vector<const CoordR3*> coord;
                    coord.reserve(3);
                    while (!stl.eof() && label != "endloop") {
                        stl >> label;
                        if (label == "vertex") {
                            CVecR3 pos;
                            stl >> pos(x) >> pos(y) >> pos(z);
                            coord.push_back(cG.get(pos));
                        }
                    }
                    eG.add(new Tri3(ElementId(0), &coord[0], lId, MatId(0)), true);
                }
            }
        }
    }

    // Stores results and returns.
    SmbData* res = new SmbData();
    res->setFilename(getFilename());
    res->mesh = new MeshUnstructured(cG, eG, lG);
    return res;
}

void
ParserSTL::printInfo() const {
    cout << " ---- Parser STL info ---- " << endl;
    cout << " --- End of Parser STL info --- " << endl;
}
