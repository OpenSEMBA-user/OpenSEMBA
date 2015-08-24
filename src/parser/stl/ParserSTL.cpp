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
ParserSTL::read(const OptionsMesher* optionsMesher) {
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
    cG.addPos(vertices);
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
            Layer* lay = lG.addId(new Layer(layerName))(0);
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
                            coord.push_back(cG.getPos(pos));
                        }
                    }
                    label.clear();
                    eG.addId(new Triangle3(ElementId(0), &coord[0], lId, MatId(0)));
                }
            }
        }
    }

    // Stores results and returns.
    SmbData* res = new SmbData();
    res->setFilename(getFilename());
    res->mesh = new MeshUnstructured(cG, eG, lG);

    if (optionsMesher == NULL) {
        res->mesherOptions = new OptionsMesher();
    } else {
        res->mesherOptions = new OptionsMesher(*optionsMesher);
    }

    res->solverOptions = new OptionsSolver();
    res->solverOptions->setSolver(OptionsSolver::Solver::none);

    res->pMGroup = new GroupPhysicalModels<>();
    MatId pecId(1);
    res->pMGroup->add(new PMPEC(pecId, "PEC"));
    res->mesh->castTo<MeshUnstructured>()->setMatId(pecId);
    res->emSources = new GroupEMSources<>();
    res->outputRequests = new GroupOutRqs<>();

    if (res->mesherOptions->isGridStepSet()) {

        CVecR3 step = res->mesherOptions->getGridStep();
        BoxR3 bound = res->mesh->castTo<MeshUnstructured>()->getBoundingBox();
        BoxR3 boundAdd2cells (bound.getMin()-step*2.0,
                              bound.getMax()+step*2.0);
        res->grid = new Grid3(boundAdd2cells, step);
    }

    return res;
}

void
ParserSTL::printInfo() const {
    cout << " ---- Parser STL info ---- " << endl;
    cout << " --- End of Parser STL info --- " << endl;
}
