/*
 * ParserGiD.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#include "ParserGiD.h"

ParserGiD::ParserGiD() {
    cG_ = NULL;
}

ParserGiD::ParserGiD(const string& fn)
:   ProjectFile(fn) {

    string null;
    init(null);
}

ParserGiD::ParserGiD(const string& fn, const string& pTPath)
:   ProjectFile(fn) {

    init(pTPath);
}

ParserGiD::~ParserGiD() {

}

SmbData*
ParserGiD::read() {
    if (!checkVersionCompatibility(readVersion())) {
        return NULL;
    }
    SmbData* res = new SmbData();
    res->setFilename(getFilename());
    res->gData = readProblemData();
    res->meshingParams = readMeshingParameters();
    pSize_ = readProblemSize();
    res->layers = readLayers();
    res->pMGroup = readMaterials();
    res->emSources = readEMSources();
    res->outputRequests = readOutputRequests();
    res->mesh = readMesh();
    res->applyGeometricScalingFactor();
    return res;
}

void
ParserGiD::printInfo() const {
    cout << "--- GiDParser info ---" << endl;
    cout << "--- End of GiDParser info ---" << endl;
}

GlobalProblemData*
ParserGiD::readProblemData() {
    GlobalProblemData* res = new GlobalProblemData();
    string line, label, value;
    bool finished = false;
    bool problemDataFound = false;
    while (!problemDataFound && !f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Problem data") == 0) {
            problemDataFound = true;
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label, value);
                if (label.compare("Final time") == 0) {
                    res->finalTime = atof(value.c_str());
                } else if (label.compare("Time step") == 0) {
                    res->timeStep = atof(value.c_str());
                } else if (label.compare("Default sampling period") == 0) {
                    res->samplingPeriod = atof(value.c_str());
                } else if (label.compare("Geometry scaling factor") == 0) {
                    res->scalingFactor = atof(value.c_str());
                } else if (label.compare("Upper x bound") == 0) {
                    res->boundTermination[0].second = strToBoundType(value);
                } else if (label.compare("Lower x bound") == 0) {
                    res->boundTermination[0].first = strToBoundType(value);
                } else if (label.compare("Upper y bound") == 0) {
                    res->boundTermination[1].second = strToBoundType(value);
                } else if (label.compare("Lower y bound") == 0) {
                    res->boundTermination[1].first = strToBoundType(value);
                } else if (label.compare("Upper z bound") == 0) {
                    res->boundTermination[2].second = strToBoundType(value);
                } else if (label.compare("Lower z bound") == 0) {
                    res->boundTermination[2].first = strToBoundType(value);
                } else if (label.compare("Boundary padding") == 0) {
                    res->boundaryPadding = strToBound(value);
                } else if (label.compare("Boundary mesh size") == 0) {
                    res->boundaryMeshSize = strToBound(value);
                } else if(label.find("End of problem data") != label.npos) {
                    finished = true;
                }
            } // Closes ( !finished && !f_in.eof() ) while.
        } // Closes problem data found if.
    } // Closes problemDataFound while.
    // Throws error messages if a problem was detected.
    if (!problemDataFound) {
        cerr<< "ERROR @ readProblemData(): "
                << "EoF was reached but problem data was not found." << endl;
    }
    if (!finished) {
        cerr<< "ERROR @ readProblemData(): "
                << "EoF reached, \"end of problem data\" not found." << endl;
    }
    //
    return res;
}

Mesh*
ParserGiD::readMesh() {
    // Read Grid3
    Grid3* grid = readCartesianGrid();
    // Reads the coordinates.
    cG_ = readCoordinates();
    // Reads elements connectivities.
    ElementsGroup elements = readElements(*cG_);
    // Builds mesh with the read data.
    return new Mesh(*cG_, elements, grid);
}

EMSourceGroup*
ParserGiD::readEMSources() {
    vector<Dipole> dipoles;
    vector<PlaneWave> pws;
    vector<Waveport> wps;
    vector<Generator> gen;
    vector<SourceOnLine> nodal;
    //
    bool finished = false;
    bool found = false;
    string label, value;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Excitations")==0) {
            found = true;
            finished = false;
            // Runs over file reading elements excitation defined.
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label,value);
                if (label.compare("Puntual excitation")==0) {
                    dipoles.push_back(readDipole());
                } else if (label.compare("Planewave")==0) {
                    pws.push_back(readPlaneWave());
                } else if (label.compare("Generator")==0) {
                    gen.push_back(readGenerator());
                } else if (label.compare("Waveport")==0) {
                    wps.push_back(readWaveport());
                } else if (label.compare("Source_on_line")==0) {
                    nodal.push_back(readSourceOnLine());
                } else if (label.compare("End of Excitations")==0) {
                    finished = true;
                } // if: end of boundary conditions label was found.
            } // while: loops until eof or the task is finished.
        } // if: boundary condition label was found.
    } // while: information was found or eof was reached.
    if (!found) {
        cerr<< "ERROR @ Parsing sources: "
                << "Excitations label was not found." << endl;
    }
    //
    return new EMSourceGroup(dipoles, pws, wps, gen, nodal);
}

PhysicalModelGroup<>*
ParserGiD::readMaterials(){
    PhysicalModelGroup<>* res = new PhysicalModelGroup<>();
    string label, value;
    uint materialCount = 0;
    while (!f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Materials")==0) {
            while (!f_in.eof() && label.compare("End of materials")!=0) {
                getNextLabelAndValue(label, value);
                if (label.compare("Material")==0) {
                    materialCount++;
                    uint id = atoi(value.c_str());
                    res->add(readPhysicalModel(id));
                }
            }
        }
    }
    assert(materialCount == pSize_.mat);
    return res;
}

PhysicalModel*
ParserGiD::readPhysicalModel(const uint id) {
    bool materialFinished = false;
    string name;
    PhysicalModelGroup<>::Type type = PhysicalModelGroup<>::undefined;
    PMMultiport::Type mpType = PMMultiport::undefined;
    SIBCType surfType = undefinedSIBC;
    string layersStr;
    double rEps, rMu, eC, mC;
    double radius, R, L, C;
    string filename;
    while (!f_in.eof() && !materialFinished) {
        string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("Name")==0) {
            name = trim(value);
        } else if (label.compare("TypeId")==0) {
            type = strToMaterialType(value);
            if (type == PhysicalModelGroup<>::multiport) {
                mpType = strToMultiportType(value);
            }
        } else if (label.compare("Permittivity")==0) {
            rEps = atof(value.c_str());
        } else if (label.compare("Permeability")==0) {
            rMu = atof(value.c_str());
        } else if (label.compare("Electric Conductivity")==0) {
            eC = atof(value.c_str());
        } else if (label.compare("Magnetic Conductivity")==0) {
            mC = atof(value.c_str());
        } else if (label.compare("Radius")==0) {
            radius = atof(value.c_str());
        } else if (label.compare("Resistance")==0) {
            R = atof(value.c_str());
        } else if (label.compare("Inductance")==0) {
            L = atof(value.c_str());
        } else if (label.compare("Capacitance")==0) {
            C = atof(value.c_str());
        } else if (label.compare("SurfaceType")==0) {
            surfType = strToSIBCType(value);
        } else if (label.compare("Filename")==0) {
            filename = value;
        } else if (label.compare("Layers")==0) {
            layersStr = value;
        } else if (label.compare("End of Material")==0) {
            // Creates material.
            switch (type) {
            case PhysicalModelGroup<>::PEC:
                return new PMPEC(id, name);
            case PhysicalModelGroup<>::PMC:
                return new PMPMC(id, name);
            case PhysicalModelGroup<>::SMA:
                return new PMSMA(id, name);
            case PhysicalModelGroup<>::PML:
                return new PMVolumePML(id, name);
            case PhysicalModelGroup<>::classic:
                if (eC == 0 && mC == 0) {
                    return new PMVolume(id, name, rEps, rMu);
                }
                return new PMVolumeDispersive(id, name, rEps, rMu, eC, mC);
            case PhysicalModelGroup<>::elecDispersive:
                return readDispersiveMatFile(id,name);
            case PhysicalModelGroup<>::isotropicsibc:
                switch (surfType) {
                case sibc:
                    return readIsotropicSurfMatFile(id, name);
                case multilayer:
                    return readMultilayerSurf(id, name, layersStr);
                default:
                    cerr << "ERROR @ ParserGiD: Undefined SIBC Type." << endl;
                }
                break;
            case PhysicalModelGroup<>::wire:
                return new PMWire(id, name, radius, R, L);
            case PhysicalModelGroup<>::multiport:
                if (mpType == PMMultiport::shortCircuit) {
                    return new PMMultiportPredefined(id, name, mpType);
                } else {
                    return new PMMultiportRLC(id, name, mpType, R, L, C);
                }
            default:
                cerr<< "ERROR @ Parsing materials: ";
                cerr<< "Material type not recognized." << endl;
            }
            materialFinished = true;
        }
    }
}

OutRqGroup*
ParserGiD::readOutputRequests() {
    OutRqGroup* res = new OutRqGroup();
    bool finished;
    bool found = false;
    string line, label, value;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Output Requests")==0) {
            found = true;
            finished = false;
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label, value);
                if (label.compare("Output request instance")==0) {
                    readOutRqInstances(res);
                } else if (label.compare("End of Output Requests")==0) {
                    finished = true;
                }
            }
        }
    }
    return res;
}

void
ParserGiD::readOutRqInstances(OutRqGroup* res) {
    GiDOutputType gidOutputType = ParserGiD::undefined;
    string line, label, value;
    bool finished = false;
    string name;
    OutRq::Type type = OutRq::undefined;
    Domain domain;
    vector<uint> elemVec;
    uint nE = 0;
    while (!finished && !f_in.eof()) {
        getNextLabelAndValue(label,value);
        if (label.compare("GiDOutputType")==0) {
            gidOutputType = strToGidOutputType(trim(value));
        } else if (label.compare("Number of elements")==0) {
            nE = atoi(value.c_str());
            for (uint i = 0; i < nE; i++) {
                getNextLabelAndValue(label,value);
                name = trim(value);
                getNextLabelAndValue(label,value);
                type = strToOutputType(trim(value));
                getNextLabelAndValue(label,value);
                domain = strToDomain(value);
                switch (gidOutputType) {
                case ParserGiD::outRqOnPoint:
                    getNextLabelAndValue(label,value);
                    elemVec.clear();
                    elemVec.push_back(atoi(value.c_str()));
                    res->add(new OutRq(
                            domain, Element::node, type, name, elemVec));
                    break;
                case ParserGiD::outRqOnLine:
                    getNextLabelAndValue(label,value);
                    elemVec.clear();
                    elemVec.push_back(atoi(value.c_str()));
                    res->add(new OutRq(domain, Element::line,
                            type, name, elemVec));
                    break;
                case ParserGiD::outRqOnSurface:
                    getNextLabelAndValue(label,value);
                    elemVec.clear();
                    elemVec.push_back(atoi(value.c_str()));
                    res->add(new OutRq(
                            domain, Element::surface,
                            type, name, elemVec));
                    break;
                case ParserGiD::outRqOnVolume:
                    getline(f_in, line);
                    res->add(
                            new OutRq(domain, Element::volume,
                            type, name,
                            BoxD3(strToBound(line))));
                    break;
                case ParserGiD::farField:
                {
                    getline(f_in, line);
                    BoxD3 bbox(strToBound(line));
                    double iTh, fTh, sTh, iPhi, fPhi, sPhi;
                    f_in >> iTh >> fTh >> sTh >> iPhi >> fPhi >> sPhi;
                    getline(f_in, line);
                    res->add(new OutRqFarField(
                            domain, Element::volume,
                            name, bbox,
                            iTh, fTh, sTh, iPhi, fPhi, sPhi));

                }
                break;
                case ParserGiD::undefined:
                    cerr<< "ERROR @ GiDParser: "
                    << "Unreckognized GiD Output request type:"
                    << type << endl;
                    break;
                }
            } // End of loop running over the elements.
        } else if (label.compare("End of Output request instance")==0) {
            finished = true;
        } else {
            cerr<< "ERROR @ GiDParser::readOutputRequestsInstance(): "
                    << "Label not identified: " << label << endl;
        } // End of condition comparing labels.
    }
}

MeshingParameters*
ParserGiD::readMeshingParameters() {
    bool finished;
    bool found = false;
    string line, label, value;
    MeshingParameters::Mesher mesher = MeshingParameters::ugrMesher;
    MeshingParameters::Mode mode = MeshingParameters::structured;
    bool bruteForceVolumes = true;
    string edgeFraction;
    string swfForce;
    bool scaleFactor = false;
    string scaleFactorValue;
    bool effectiveParameters = false;
    string thickness, sigma;
    bool locationInMeshIsSet = false;
    CVecD3 locationInMesh;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Meshing parameters") == 0) {
            found = true;
            finished = false;
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label, value);
                if (label.compare("Mesher") == 0) {
                    mesher = strToMesher(value);
                } else if (label.compare("Brute force volumes") == 0) {
                    bruteForceVolumes = strToBool(value);
                } else if (label.compare("Mode") == 0) {
                    mode = strToMesherMode(value);
                } else if (label.compare("Edge fraction") == 0) {
                    edgeFraction = trim(value);;
                } else if (label.compare("SWF force") == 0) {
                    swfForce = trim(value);
                } else if (label.compare("Scale factor") == 0) {
                    scaleFactor = strToBool(value);
                } else if (label.compare("Scale factor value") == 0) {
                    scaleFactorValue = trim(value);
                } else if (label.compare("Effective parameters") == 0) {
                    effectiveParameters = strToBool(value);
                } else if (label.compare("Thickness") == 0) {
                    thickness = trim(value);
                } else if (label.compare("Sigma") == 0) {
                    sigma = trim(value);;
                } else if (label.compare("Location in mesh")==0) {
                    CoordinateId id(atoi(value.c_str()));
                    const CoordinateBase* coord = cG_->getPtrToId(id);
                    if(coord != NULL) {
                        if(coord->isOf<CoordD3>()) {
                            locationInMeshIsSet = true;
                            locationInMesh = coord->castTo<CoordD3>()->pos();
                        }
                    }

                } else if (label.compare("End of Meshing parameters")==0) {
                    finished = true;
                }
            }
        }
    }
    //
    return new MeshingParameters(mesher, locationInMeshIsSet, locationInMesh,
            bruteForceVolumes, mode, effectiveParameters, thickness, sigma,
            edgeFraction, scaleFactor, scaleFactorValue, swfForce,
            getProjectFolder() + getProjectName());
}

ProblemSize
ParserGiD::readProblemSize() {
    string label, line, value;
    bool finished = false;
    bool problemSizeFound = false;
    ProblemSize res;
    // Runs until "Problem Size" is found or eof is reached.
    while (!problemSizeFound && !f_in.eof()) {
        getline(f_in, line);
        if (line.find("Problem size:") != line.npos ) {
            problemSizeFound = true;
            while(!finished) {
                getNextLabelAndValue(label, value);
                if (label.compare("Hexahedral elements")==0) {
                    res.hex8 = atoi(value.c_str());
                } else if (label.compare("Quadratic Tetrahedral elements")==0) {
                    res.tet10 = atoi(value.c_str());
                } else if (
                        label.compare("Linear Tetrahedral elements")==0 ||
                        label.compare("Tetrahedral elements")==0) {
                    res.tet4 = atoi(value.c_str());
                } else if (label.compare("Quadratic Triangular elements")==0) {
                    res.tri6 = atoi(value.c_str());
                } else if (label.compare("Linear Triangular elements")==0
                        || label.compare("Triangular elements")==0) {
                    res.tri3 = atoi(value.c_str());
                } else if (label.compare("Quadratic Line elements")==0) {
                    res.lin3 = atoi(value.c_str());
                } else if (
                        label.compare("Linear Line elements")==0 ||
                        label.compare("Line elements")==0) {
                    res.lin2 = atoi(value.c_str());
                } else if (label.compare("Number of points")==0) {
                    res.v = atoi(value.c_str());
                } else if (label.compare("Number of materials")==0) {
                    res.mat = atoi(value.c_str());
                } else if(label.find("End of problem size") != label.npos) {
                    finished = true;
                }
                if (f_in.eof()) {
                    cerr<< "ERROR @ Parser::readProblemSize(): "
                            << "End of problem size label not found" << endl;
                }
            }
        }
    }
    if (!problemSizeFound) {
        cerr<< "ERROR @ Parser::readProblemSize: "
                << "Problem size label not found."     << endl;
    }
    return res;
}

LayerGroup*
ParserGiD::readLayers() {
    bool finished = false;
    bool found = false;
    string label, value;
    uint id;
    LayerGroup* res = new LayerGroup();
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Layers")==0) {
            found = true;
            while(!finished && !f_in.eof()) {
                string line;
                getline(f_in, line);
                if (line.find("End of layers") != line.npos) {
                    finished = true;
                } else {
                    stringstream ss(line);
                    ss >> id >> value;
                    res->add(new Layer(id, value));
                }
            }
        }
    }
    if (!found) {
        cerr<< "ERROR @ Parsing layers: "
                << "Layers label was not found." << endl;
        return NULL;
    }
    return res;
}

CoordinateGroup<>*
ParserGiD::readCoordinates() {
    string line;
    uint id;
    CVecD3 pos;
    vector<CoordinateBase*> coord;
    coord.reserve(pSize_.v);
    bool finished = false;
    bool found = false;
    while (!found && !f_in.eof() && !finished) {
        getline(f_in, line);
        if (line.find("Coordinates:") != line.npos) {
            found = true;
            // Reads coordinates.
            for (uint i = 0; i < pSize_.v; i++) {
                f_in >> id >> pos(0) >> pos(1) >> pos(2);
                coord.push_back(new Coordinate<double,3>(CoordinateId(id),pos));
            }
            // Checks "end of coordinates" label.
            finished = false;
            while(!finished && !f_in.eof()) {
                getline(f_in, line);
                if (line.find("End of coordinates") != line.npos) {
                    finished = true;
                }
            }
        }
    }
    if (!found) {
        cerr<< "ERROR @ GiDParser::readCoordinates(): "
                << "Coordinates label was not found." << endl;
    }
    if (!finished) {
        cerr<< "ERROR @ GiDParser::readCoordinates(): "
                << "End of coordinates label not found." << endl;
    }

    CoordinateGroup<>* cG = new CoordinateGroup<>(coord);

    for(unsigned i = 0; i < coord.size(); i++)
        delete coord[i];

    return cG;
}


ElementsGroup
ParserGiD::readElements(const CoordinateGroup<>& v) {
    string line, label;
    bool finished = false;
    bool found = false;
    vector<Lin2> lin2;
    vector<Tri3> tri3;
    vector<Tri6> tri6;
    vector<Tet4> tet4;
    vector<Tet10> tet10;
    vector<Hex8> hex8;
    while (!finished && !f_in.eof()) {
        getline(f_in, line);
        if (line.find("Elements:") != line.npos) {
            found = true;
        }
        // Stores data label in labelStr string.
        label = line.substr(0, line.find(LABEL_ENDING));
        if (label.compare("Linear Hexahedral Elements")==0 ||
                label.compare("Hexahedral Elements")==0) {
            hex8 = readHex8Elements(v);
        } else if (label.compare("Quadratic Tetrahedral Elements")==0) {
            tet10 = readTet10Elements(v);
        } else if (label.compare("Linear Tetrahedral Elements")==0 ||
                label.compare("Tetrahedral Elements")==0) {
            tet4 = readTet4Elements(v);
        } else if (label.compare("Quadratic Triangle Elements")==0) {
            tri6 = readTri6Elements(v);
        } else if (label.compare("Linear Triangle Elements")==0 ||
                label.compare("Triangle Elements")==0) {
            tri3 = readTri3Elements(v);
        } else if (label.compare("Linear Line Elements")==0 ||
                label.compare("Line Elements")==0) {
            lin2 = readLin2Elements(v);
        } else if(label.find("End of Elements") != label.npos) {
            finished = true;
        }
    }
    // Shows error message if the elements label was not found.
    if (!found) {
        cerr<< "ERROR @ GiDParser::readElements(): "
                << "\"Elements\" label was not found." << endl;
    }
    // This code is reached only in case of "End of elements" is not found.
    if (!finished) {
        cerr<< "ERROR @ GiDParser::readElements()"
                << "\"End of elements\" label was not found." << endl;
    }
    //
    ElementsGroup res(lin2, tri3, tri6, tet4, tet10, hex8);
    return res;
}

vector<Hex8>
ParserGiD::readHex8Elements(const CoordinateGroup<>& v) {
    vector<Hex8> res;
    ElementId id;
    uint matId;
    CoordinateId vId[8];
    res.reserve(pSize_.hex8);
    for (uint i = 0; i < pSize_.hex8; i++) {
        f_in >> id;
        for (uint j = 0; j < 8; j++) {
            f_in >> vId[j];
        }
        f_in >> matId;
        res.push_back(Hex8(v, id, vId, 0, matId));
    }
    return res;
}

vector<Tet10>
ParserGiD::readTet10Elements(const CoordinateGroup<>& v) {
    vector<Tet10> res;
    ElementId id;
    uint matId;
    CoordinateId vId[10];
    res.reserve(pSize_.tet10);
    for (uint i = 0; i < pSize_.tet10; i++) {
        f_in >> id;
        for (uint j = 0; j < 10; j++) {
            f_in >> vId[j];
        }
        f_in >> matId;
        res.push_back(Tet10(v, id, vId, 0, matId));
    }
    return res;
}

vector<Tet4>
ParserGiD::readTet4Elements(
        const CoordinateGroup<>& v) {
    vector<Tet4> res;
    ElementId id;
    uint matId, layerId;
    CoordinateId vId[4];
    res.reserve(pSize_.tet4);
    for (uint i = 0; i < pSize_.tet4; i++) {
        f_in >> id >> vId[0] >> vId[1] >> vId[2] >> vId[3] >> matId >> layerId;
        res.push_back(Tet4(v, id, vId, layerId, matId));
    }
    return res;
}

vector<Tri6>
ParserGiD::readTri6Elements(const CoordinateGroup<>& v) {
    vector<Tri6> res;
    ElementId id;
    uint matId;
    CoordinateId vId[6];
    CVecD3 normal;
    res.reserve(pSize_.tri6);
    for (uint i = 0; i < pSize_.tri6; i++) {
        f_in >> id;
        for (uint j = 0; j < 6; j++)
            f_in >> vId[j];
        f_in >> matId;
        res.push_back(Tri6(v, id, vId, 0, matId));
    }
    return res;
}


vector<Tri3>
ParserGiD::readTri3Elements(const CoordinateGroup<>& v) {
    vector<Tri3> res;
    ElementId id;
    uint matId, layerId;
    CoordinateId vId[3];
    CVecD3 normal;
    res.reserve(pSize_.tri3);
    for (uint i = 0; i < pSize_.tri3; i++) {
        f_in >> id >> vId[0] >> vId[1] >> vId[2] >> matId >> layerId;
        res.push_back(Tri3(v, id, vId, layerId, matId));
    }
    return res;
}

vector<Lin2>
ParserGiD::readLin2Elements(const CoordinateGroup<>& v) {
    vector<Lin2> res;
    ElementId id;
    uint matId, layerId;
    CoordinateId vId[2];
    res.reserve(pSize_.lin2);
    for (uint i = 0; i < pSize_.lin2; i++) {
        f_in >> id >> vId[0] >> vId[1] >> matId >> layerId;
        res.push_back(Lin2(v, id, vId, layerId, matId));
    }
    return res;
}

PMVolumeDispersive*
ParserGiD::readDispersiveMatFile(
        const uint id_, const string& fileName) const {
    ifstream matFile;
    string matFileName, line, label, value;
    string name, model;
    string poles, epsilon, sigma;
    uint nPoles, nDrudePoles;
    double eps, sig;
    vector<int> poleId, drudePoleId;
    vector<double> rePK, imPK, reRK, imRK;
    vector<double> reDrudePK, imDrudePK, reDrudeRK, imDrudeRK;
    int tmpPoleId;
    double tmpRePK, tmpImPK, tmpReRK, tmpImRK;
    // Opens file, read only mode.
    matFileName = problemTypePath_ + "/material/" + fileName + ".dat";
    matFile.open(matFileName.c_str(), ifstream::in);
    if (matFile.fail()) {
        cerr<< "ERROR @ readDispersiveMaterialFile()"
                << "Problem opening file: " << matFileName << endl;
    }
    // Parses first line, containing material name.
    getline(matFile, line);
    label = line.substr(line.find("#") + 2, line.find(":") - 2);
    name  = label.substr(0, label.find(" "));
    model = label.substr(label.find(" ") + 1, label.length());
    value = line.substr(line.find(":") + 2, line.length());
    poles = value.substr(0, value.find("-"));
    nPoles = atoi(poles.c_str());
    epsilon = value.substr(value.find("epsInf=") + 7, 8);
    eps = atof(epsilon.c_str());
    sigma = value.substr(value.find("Sigma=") + 6, 8);
    sig = atof(sigma.c_str());
    // Parses poles.
    // Stores in line the file line containing headers.
    getline(matFile, line);
    for (uint i = 0; i < nPoles; i++) {
        matFile >> tmpPoleId >> tmpRePK >> tmpImPK >> tmpReRK >> tmpImRK;
        poleId.push_back(tmpPoleId);
        rePK.push_back(tmpRePK);
        imPK.push_back(tmpImPK);
        reRK.push_back(tmpReRK);
        imRK.push_back(tmpImRK);
    }
    getline(matFile, line);
    label = line.substr(line.find("#") + 2, line.find(":") - 2);
    value = line.substr(line.find(":") + 2, line.length());
    nDrudePoles = atoi(value.c_str());
    for (uint i = 0; i < nDrudePoles; i++) {
        matFile >> tmpPoleId >> tmpRePK >> tmpImPK >> tmpReRK >> tmpImRK;
        drudePoleId.push_back(tmpPoleId);
        reDrudePK.push_back(tmpRePK);
        imDrudePK.push_back(tmpImPK);
        reDrudeRK.push_back(tmpReRK);
        imDrudeRK.push_back(tmpImRK);
    }
    // Copies all parsed data into the aux material depending on the model.
    if (!model.compare("Pole-Residue Model")) {
        vector<complex<double> > poles, residues, drudePoles, drudeResidues;
        for (uint i = 0; i < nPoles; i++) {
            complex<double> pole(rePK[i], imPK[i]);
            poles.push_back(pole);
            complex<double> residue(reRK[i]/2.0, imRK[i]/2.0);
            residues.push_back(residue);
        }
        for (uint i = 0; i < nDrudePoles; i++) {
            complex<double> pole(reDrudePK[i], imDrudePK[i]);
            drudePoles.push_back(pole);
            complex<double> residue(reDrudeRK[i]/2.0, imDrudeRK[i]/2.0);
            drudeResidues.push_back(residue);
        }
        return new PMVolumeDispersive(id_, name, eps, 1.0, sig,
                poles, residues, drudePoles, drudeResidues);
    }
    cerr<< "ERROR@GiDParser::readDispersiveMaterialFile(...)"
            << "File contains unknown model." << endl;
    return NULL;
}

PMSurfaceMultilayer*
ParserGiD::readMultilayerSurf(
        const int id,
        const string& name,
        const string& layersStr) const {
    uint begin = layersStr.find_first_of("\"");
    uint end = layersStr.find_last_of("\"");
    istringstream ss(layersStr.substr(begin+1,end-2));
    string sub;
    vector<double> thick, rEps, rMu, eCond, mCond;
    uint parameters;
    string trash;
    ss >> trash >> parameters;
    const uint nLayers = parameters / 5;
    for (uint i = 0; i < nLayers; i++) {
        // Thickness, Permittivity, Permeability, ElecCond, MagnCond.
        double thick_, rEps_, rMu_, eCond_, mCond_;
        ss >> thick_ >> rEps_ >> rMu_ >> eCond_ >> mCond_;
        thick.push_back(thick_);
        rEps.push_back(rEps_);
        rMu.push_back(rMu_);
        eCond.push_back(eCond_);
        mCond.push_back(mCond_);
    }
    return new PMSurfaceMultilayer(id, name, thick, rEps, rMu, eCond, mCond);
}

PMSurfaceSIBC*
ParserGiD::readIsotropicSurfMatFile(
        const int id_, const string& fileName) const {
    ifstream matFile;
    string matFileName, line, label, value;
    string name, model;
    char *pEnd;
    StaMatrix<double,2,2> Zstatic, Zinfinite;
    vector<double> pole;
    vector<StaMatrix<double,2,2> > Z;
    double tmpP, tmpZ11, tmpZ12, tmpZ21, tmpZ22;
    // Opens file, read only mode.
    matFileName = problemTypePath_ + "/panel/" + fileName + ".dat";
    matFile.open(matFileName.c_str(), ifstream::in);
    if (matFile.fail()) {
        cerr<< "ERROR @ readSurfaceMaterialFile(): "
                << "Problem opening file: " << matFileName << endl;
    }
    // Parses first line, containing material name.
    getline(matFile, line);
    if (line.find("#PANEL#") == string::npos) {
        cerr<< "ERROR @ Parser::readSurfaceMaterialFile(...)"
                << "File: " << matFileName << "   "
                << "#PANEL# label has not been found in first line" << endl;
    }
    name = line.substr(8, line.length()-9);
    getline(matFile, line);
    uint nPoles = 0;
    // Gets number of poles.
    label = line.substr(0, line.find(":"));
    if(!label.compare("N")) {
        value  = line.substr(line.find(":")+2, line.length());
        nPoles = atoi(value.c_str());
    }
    // Gets Zinfinite.
    getline(matFile, line);
    label = line.substr(0, line.find(":"));
    if(!label.compare("Zinf")) {
        value  = line.substr(line.find(":")+2, line.length());
        Zinfinite(0,0) = strtod(value.c_str(), &pEnd);
        Zinfinite(0,1) = strtod(pEnd, &pEnd);
        Zinfinite(1,0) = strtod(pEnd, &pEnd);
        Zinfinite(1,1) = strtod(pEnd, &pEnd);
    }
    // Gets Zstatic.
    getline(matFile, line);
    label = line.substr(0, line.find(":"));
    if(!label.compare("Zstatic")) {
        value  = line.substr(line.find(":")+2, line.length());
        Zstatic(0,0) = strtod(value.c_str(), &pEnd);
        Zstatic(0,1) = strtod(pEnd, &pEnd);
        Zstatic(1,0) = strtod(pEnd, &pEnd);
        Zstatic(1,1) = strtod(pEnd, &pEnd);
    }
    // Parses poles.
    // Stores in line the file line containing headers.
    getline(matFile, line);
    for (uint i = 0; i < nPoles; i++) {
        matFile >> tmpP >> tmpZ11 >> tmpZ12 >> tmpZ21 >> tmpZ22;
        StaMatrix<double,2,2> tmpZ;
        tmpZ(0,0) = tmpZ11;
        tmpZ(0,1) = tmpZ12 ;
        tmpZ(1,0) = tmpZ21;
        tmpZ(1,1) = tmpZ22;
        pole.push_back(tmpP);
        Z.push_back(tmpZ);
    }
    // Copies all parsed data into the aux material depending on the model.
    return new PMSurfaceSIBC (id_, name, Zinfinite, Zstatic, pole, Z);
}

void
ParserGiD::getNextLabelAndValue(string& label, string& value) {
    string line;
    getline(f_in, line);
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
    label = line.substr(0, line.find(LABEL_ENDING));
    value = line.substr(line.find(LABEL_ENDING) + 1, line.length());
}

Grid3*
ParserGiD::readCartesianGrid() {
    string label, line, value;
    bool finished = false;
    bool gridLabelFound = false;
    bool gridFound = false;
    Grid3* grid = NULL;
    BoxD3 bound;
    bool stepsByNumberOfCells = true;
    CVecI3 numElems;
    CVecD3 steps;
    while (!gridLabelFound && !f_in.eof()) {
        getline(f_in, line);
        if (line.find("Grid:") != line.npos ) {
            gridLabelFound = true;
            while(!finished) {
                getNextLabelAndValue(label, value);
                if (label.compare("Layer Box")==0) {
                    gridFound = true;
                    bound = BoxD3(strToBound(value));
                } else if (label.compare("Type")==0) {
                    if (trim(value).compare("by_number_of_cells")==0) {
                        stepsByNumberOfCells = true;
                    } else {
                        stepsByNumberOfCells = false;
                    }
                } else if (label.compare("Directions")==0) {
                    CVecD3 aux = strToCartesianVector(value);
                    if (stepsByNumberOfCells) {
                        numElems(0) = (long) aux(0);
                        numElems(1) = (long) aux(1);
                        numElems(2) = (long) aux(2);
                    } else {
                        steps = aux;
                    }
                } else if(label.find("End of Grid") != label.npos) {
                    finished = true;
                    if (!gridFound) {
                        return NULL;
                    }
                }
                if (f_in.eof()) {
                    cerr<< "ERROR @ ParserGiD()"
                            << "End of grid label not found" << endl;
                }
            }
        }
    }
    // Throws error message if label was not found.
    if (!gridLabelFound) {
        cerr<< "ERROR @ ParserGiD: "
                << "Grid3 label not found." << endl;
    }
    if (gridFound) {
        if (stepsByNumberOfCells) {
            grid = new Grid3(bound, numElems);
        } else {
            grid = new Grid3(bound, steps);
        }
    } else {
        grid = NULL;
    }
    return grid;
}

void
ParserGiD::init(const string& pTPath) {
    problemTypePath_ = pTPath;
    struct stat st;
    if (stat(getFilename().c_str(), &st) == 0) {
        if (st.st_mode & S_IFDIR) {
            cerr<< "ERROR@GiDParser::GiDParser(): "
                    << getFilename() << "It is a directory " << endl;
        }  else if(st.st_mode & S_IFREG) {
            f_in.open(getFilename().c_str(), ifstream::in);
            if (f_in.fail()) {
                cerr<< "ERROR @ ParserGiD::GiDParser(): "
                        << "Problem opening file: " << getFilename() << endl;
            }
            return;
        }
    }
}

PlaneWave
ParserGiD::readPlaneWave() {
    string filename;
    string label, value;
    CVecD3 dir, pol;
    pair<CVecD3,CVecD3> bound;
    vector<uint> elems;
    Magnitude* mag;
    bool isDefinedOnLayer = true;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Direction")==0) {
            dir = strToCartesianVector(value);
        } else if (label.compare("Polarization") == 0) {
            pol = strToCartesianVector(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Layer Box") == 0) {
            bound = strToBound(value);
        } else if (label.compare("Number of elements")==0) {
            isDefinedOnLayer = false;
            uint nE = atoi(value.c_str());
            elems.reserve(nE);
            for (uint i = 0; i < nE; i++) {
                uint e;
                f_in >> e;
                elems.push_back(e);
            }
        } else if (label.compare("End of Planewave")==0) {
            if (isDefinedOnLayer) {
                return PlaneWave(bound,	 dir, pol, mag);
            } else {
                return PlaneWave(elems, dir, pol, mag);
            }
        }
    }
    // Throws error message if ending label was not found.
    cerr<< "ERROR @ Parsing planewave: "
            << "End of Planewave label not found. " << endl;
    return PlaneWave();
}

Dipole
ParserGiD::readDipole() {
    vector<uint> elem;
    double length = 0.0;
    CVecD3 orientation;
    CVecD3 position;
    MagnitudeGaussian* mag = NULL;
    //
    string line;
    bool finished = false;
    char* pEnd;
    while(!finished && !f_in.eof()) {
        getline(f_in, line);
        if (line.find("End of puntual excitation") == line.npos) {
            elem.push_back(strtol(line.c_str(), &pEnd, 10 ));
            if (elem.size() == 1) {
                assert(false); // TODO: Not implemented.
            }
        } else
            finished = true;
    }
    if (!finished) {
        cerr<< "ERROR @ ParserGiD::readDipoleEMSource: "
                << "End of excitation type label not found. "
                << endl;
    }
    //
    Dipole res(elem, length, orientation, position, mag);
    return res;
}

Waveport
ParserGiD::readWaveport() {
    vector<uint> elem;
    uint numElements = 0;
    bool input = true;
    MagnitudeGaussian* mag;
    Waveport::Shape shape = Waveport::rectangular;
    Waveport::ExcitationMode excitationMode = Waveport::TE;
    pair<uint,uint> mode(1,0);
    string line, label, value;
    bool finished = false;
    while (!finished && !f_in.eof()) {
        getline(f_in, line);
        label = line.substr(0, line.find(LABEL_ENDING));
        value = line.substr(line.find(LABEL_ENDING)+1, line.length());
        if (!label.compare("Input")) {
            uint oneOrZero;
            oneOrZero = atoi(value.c_str());
            if (oneOrZero == 1) {
                input = true;
            } else {
                input = false;
            }
        } else if (!label.compare("Shape")) {
            if (value.find("Rectangular") != value.npos) {
                shape = Waveport::rectangular;
            } else {
                cout << "ERROR @ Unreckognized waveport shape." << endl;
                exit(-1);
            }
        } else if (label.compare("Excitation") == 0) {
            mag = dynamic_cast<MagnitudeGaussian*>(readMagnitude(value));
        } else if (!label.compare("ExcitationMode")) {
            if (value.find("TE") != value.npos) {
                excitationMode = Waveport::TE;
            } else if (value.find("TM") != value.npos) {
                excitationMode = Waveport::TM;
            }
        } else if (!label.compare("FirstMode")) {
            mode.first = atoi(value.c_str());
        } else if (!label.compare("SecondMode")) {
            mode.second = atoi(value.c_str());
        } else if (!label.compare("Number of elements")) {
            numElements = atoi(value.c_str());
        } else if (!label.compare("Elements")) {
            uint e;
            for (uint i = 0; i < numElements; i++) {
                f_in >> e;
                elem.push_back(e);
            }
        } else if (label.find("End of Waveport") != label.npos) {
            finished = true;
        }
        if (f_in.eof()) {
            cerr<< "ERROR @ Parser: "
                    << "End of Waveport not found" << endl;
        }
    }
    // Throws error message if finished was not updated.
    if (!finished) {
        cerr<< "ERROR @ GiDParser::readWaveportEMSource: "
                << "End of excitation type label not found. " << endl;
    }
    return Waveport(elem, mag, input, shape, excitationMode, mode);
}

Generator
ParserGiD::readGenerator() {
    Generator::Type type;
    Generator::Hardness hardness;
    Magnitude* mag;
    vector<uint> elems;
    string filename;
    string label, value;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Type")==0) {
            type = strToGeneratorType(value);
        } else if (label.compare("Hardness")==0) {
            hardness = strToGeneratorHardness(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Number of elements")==0) {
            uint nE = atoi(value.c_str());
            elems.reserve(nE);
            for (uint i = 0; i < nE; i++) {
                uint e;
                f_in >> e;
                elems.push_back(e);
            }
        } else if (label.compare("End of Generator")==0) {
            return Generator(type, hardness, elems, mag);
        }
    }
    // Throws error message if ending label was not found.
    cerr<< "ERROR @ Parsing generator: "
            << "End of Generator label not found. " << endl;
    return Generator();
}

SourceOnLine
ParserGiD::readSourceOnLine() {
    SourceOnLine::Type type;
    SourceOnLine::Hardness hardness;
    Magnitude* mag;
    vector<uint> elems;
    string filename;
    string label, value;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Type")==0) {
            type = strToNodalType(value);
        } else if (label.compare("Hardness")==0) {
            hardness = strToNodalHardness(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Number of elements")==0) {
            uint nE = atoi(value.c_str());
            elems.reserve(nE);
            for (uint i = 0; i < nE; i++) {
                uint e;
                f_in >> e;
                elems.push_back(e);
            }
        } else if (label.compare("End of Source_on_line")==0) {
            return SourceOnLine(type, hardness, elems, mag);
        }
    }
    // Throws error message if ending label was not found.
    cerr<< "ERROR @ Parsing nodal: "
            << "End of Nodal label not found. " << endl;
    return SourceOnLine();
}

Element::Type
ParserGiD::strToElementType(string str) const {
    str = trim(str);
    if (str.compare("point")==0) {
        return Element::node;
    } else if (str.compare("line")==0) {
        return Element::line;
    } else if (str.compare("surface")==0) {
        return Element::surface;
    } else if (str.compare("volume")==0) {
        return Element::volume;
    } else {
        cerr<< "ERROR @ GiDParser::readOutputRequestInstance(): "
                << "Unreckognized element type: " << str << endl;
        return Element::undefined;
    }
}

OutRq::Type
ParserGiD::strToOutputType(string str) const {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return OutRq::electricField;
    } else if (str.compare("magneticField")==0) {
        return OutRq::magneticField;
    } else if (str.compare("electricFieldNormals")==0) {
        return OutRq::electricFieldNormals;
    } else if (str.compare("magneticFieldNormals")==0) {
        return OutRq::magneticFieldNormals;
        //	} else if (str.compare("powerDensity")==0) {
        //		return OutputRequest::powerDensity;
        //	} else if (str.compare("power")==0) {
        //		return OutputRequest::power;
    } else if (str.compare("current")==0) {
        return OutRq::current;
    } else if (str.compare("voltage")==0) {
        return OutRq::voltage;
    } else if (str.compare("bulkCurrentElectric")==0) {
        return OutRq::bulkCurrentElectric;
    } else if (str.compare("bulkCurrentMagnetic")==0) {
        return OutRq::bulkCurrentMagnetic;
    } else if (str.compare("farField")==0) {
        return OutRq::farField;
    } else {
        cerr<< "ERROR @ GiDParser::readOutputRequestInstance(): "
                << "Unrecognized output type: " << str << endl;
        return OutRq::undefined;
    }
}

ParserGiD::SIBCType
ParserGiD::strToSIBCType(string str) const {
    str = trim(str);
    if (str.compare("File")==0) {
        return sibc;
    } else if (str.compare("Layers")==0) {
        return multilayer;
    } else {
        cerr<< "ERROR @ GiDParser: "
                << "Unrecognized SIBC type: " << str << endl;
        return undefinedSIBC;
    }
}

Generator::Type
ParserGiD::strToGeneratorType(string str) const {
    str = trim(str);
    if (str.compare("voltage")==0) {
        return Generator::voltage;
    } else if (str.compare("current")==0) {
        return Generator::current;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized generator type." << endl;
        return Generator::undefined;
    }
}

Generator::Hardness
ParserGiD::strToGeneratorHardness(string str) const {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Generator::soft;
    } else if (str.compare("hard")==0) {
        return Generator::hard;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized generator hardness." << endl;
        return Generator::soft;
    }
}

GlobalProblemData::boundType
ParserGiD::strToBoundType(string str) const {
    str = trim(str);
    if (str.compare("PEC")==0) {
        return GlobalProblemData::pec;
    } else if (str.compare("PMC")==0) {
        return GlobalProblemData::pmc;
    } else if (str.compare("PML")==0) {
        return GlobalProblemData::pml;
    } else if (str.compare("Periodic")==0) {
        return GlobalProblemData::periodic;
    } else if (str.compare("MUR1")==0) {
        return GlobalProblemData::mur1;
    } else if (str.compare("MUR2")==0) {
        return GlobalProblemData::mur2;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized bound label." << endl;
        return GlobalProblemData::undefined;
    }
}

PhysicalModelGroup<>::Type
ParserGiD::strToMaterialType(string str) const {
    str = trim(str);
    if (str.compare("PEC")==0) {
        return PhysicalModelGroup<>::PEC;
    } else if (str.compare("PMC")==0) {
        return PhysicalModelGroup<>::PMC;
    } else if (str.compare("PML")==0) {
        return PhysicalModelGroup<>::PML;
    } else if (str.compare("SMA")==0) {
        return PhysicalModelGroup<>::SMA;
    } else if (str.compare("Classic")==0) {
        return PhysicalModelGroup<>::classic;
    } else if (str.compare("Dispersive")==0) {
        return PhysicalModelGroup<>::elecDispersive;
    } else if (str.compare("SIBC")==0) {
        return PhysicalModelGroup<>::isotropicsibc;
    } else if (str.compare("Wire")==0) {
        return PhysicalModelGroup<>::wire;
    } else if (str.find("Conn_") != string::npos) {
        return PhysicalModelGroup<>::multiport;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized material label." << endl;
        return PhysicalModelGroup<>::undefined;
    }
}

PMMultiport::Type
ParserGiD::strToMultiportType(string str) const {
    str = trim(str);
    if (str.compare("Conn_short")==0) {
        return PMMultiport::shortCircuit;
    } else if (str.compare("Conn_open")==0) {
        return PMMultiport::openCircuit;
    } else if (str.compare("Conn_matched")==0) {
        return PMMultiport::matched;
    } else if (str.compare("Conn_sRLC")==0) {
        return PMMultiport::sRLC;
    } else if (str.compare("Conn_pRLC")==0) {
        return PMMultiport::pRLC;
    } else if (str.compare("Conn_sLpRC")==0) {
        return PMMultiport::sLpRC;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized multiport label." << endl;
        return PMMultiport::undefined;
    }
}

pair<CVecD3, CVecD3>
ParserGiD::strToBound(
        const string& value) const {
    uint begin = value.find_first_of("{");
    uint end = value.find_last_of("}");
    istringstream iss(value.substr(begin+1,end-2));
    string sub;
    CVecD3 max, min;
    for (uint i = 0; i < 3; i++) {
        iss >> sub;
        max(i) = atof(sub.c_str());
    }
    for (uint i = 0; i < 3; i++) {
        iss >> sub;
        min(i) = atof(sub.c_str());
    }
    pair<CVecD3,CVecD3> bound(min, max);
    return bound;
}


SourceOnLine::Type
ParserGiD::strToNodalType(string str) const {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return SourceOnLine::electricField;
    } else if (str.compare("magneticField")==0) {
        return SourceOnLine::magneticField;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized nodal type." << endl;
        return SourceOnLine::undefined;
    }
}

SourceOnLine::Hardness
ParserGiD::strToNodalHardness(string str) const {
    str = trim(str);
    if (str.compare("soft")==0) {
        return SourceOnLine::soft;
    } else if (str.compare("hard")==0) {
        return SourceOnLine::hard;
    } else {
        cerr<< "ERROR @ Parser: "
                << "Unreckognized nodal hardness." << endl;
        return SourceOnLine::soft;
    }
}

string
ParserGiD::readVersion() {
    string line, label, value;
    bool formatFound = false;
    bool versionFound = false;
    string format, version;
    while ((!formatFound || !versionFound) && !f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Format") == 0) {
            formatFound = true;
            format = trim(value);
        }
        if (label.compare("Version") == 0) {
            versionFound = true;
            version = trim(value);
        }
    }
    // Shows error messages.
    if (!formatFound) {
        cerr<< "ERROR @ ParserGiD: "
                << "EoF was reached but format label was not found." << endl;
    }
    if (!versionFound) {
        cerr<< "ERROR @ ParserGiD: "
                << "EoF was reached but version label was not found." << endl;
    }
    return version;
}

ParserGiD::GiDOutputType
ParserGiD::strToGidOutputType(string str) const {
    str = trim(str);
    if (str.compare("OutRq_on_point")==0) {
        return ParserGiD::outRqOnPoint;
    } else if (str.compare("OutRq_on_line")==0) {
        return ParserGiD::outRqOnLine;
    } else if (str.compare("OutRq_on_surface")==0) {
        return ParserGiD::outRqOnSurface;
    } else if (str.compare("OutRq_on_volume")==0) {
        return ParserGiD::outRqOnVolume;
    } else if (str.compare("farField")) {
        return ParserGiD::farField;
    } else {
        cerr<< "ERROR @ Parser: ";
        cerr<< "Unreckognized label." << endl;
        return ParserGiD::undefined;
    }
}

Domain
ParserGiD::strToDomain(string line) const {
    uint timeDomain;
    double initialTime;
    double finalTime;
    double samplingPeriod;
    uint frequencyDomain;
    double initialFrequency;
    double finalFrequency;
    double frequencyStep;
    uint logFrequencySweep;
    uint usingTransferFunction;
    string transferFunctionFile;
    stringstream ss(line);
    ss >> timeDomain >> initialTime >> finalTime >> samplingPeriod
    >> frequencyDomain >> initialFrequency >> finalFrequency >>frequencyStep
    >> logFrequencySweep >> usingTransferFunction >> transferFunctionFile;
    return (Domain(toBool(timeDomain), initialTime, finalTime, samplingPeriod,
            toBool(frequencyDomain), initialFrequency, finalFrequency,
            frequencyStep,	toBool(logFrequencySweep),
            toBool(usingTransferFunction), transferFunctionFile));
}

Magnitude*
ParserGiD::readMagnitude(const string typeIn) {
    string type = typeIn;
    type = trim(type);
    bool finished = false;
    string label, value;
    if (type.compare("Gaussian") == 0) {
        double delay, spread;
        bool spreadFound = false;
        bool delayFound = false;
        while (!finished && !f_in.eof()) {
            getNextLabelAndValue(label, value);
            if (label.compare("Gaussian spread") == 0) {
                spread = atof(value.c_str());
                spreadFound = true;
            } else if (label.compare("Gaussian delay") == 0) {
                delay = atof(value.c_str());
                delayFound = true;
            }
            finished = spreadFound && delayFound;
            if (finished) {
                return new MagnitudeGaussian(spread, delay);
            }
        }
    } else if (type.compare("File") == 0) {
        string excName;
        while (!finished && !f_in.eof()) {
            getNextLabelAndValue(label, value);
            if (label.compare("Filename") == 0) {
                excName = trim(value);
                finished = true;
            }
            if (finished) {
                return new MagnitudeNumerical(getProjectFolder() + excName);
            }
        }
    }
    cerr<< "ERROR @ readMagnitude: "
            << "Unable to recognize magnitude type when reading excitation."
            << endl;
    return NULL;
}

MeshingParameters::Mesher ParserGiD::strToMesher(string str) const {
    str = trim(str);
    if (str.compare("ugrMesher")==0) {
        return MeshingParameters::ugrMesher;
    } else if (str.compare("OpenFOAM")==0) {
        return MeshingParameters::openfoam;
    } else if (str.compare("None")==0) {
        return MeshingParameters::none;
    } else {
        cerr<< "ERROR @ Parser: ";
        cerr<< "Unreckognized label: " << str<< endl;
        return MeshingParameters::none;
    }
}

MeshingParameters::Mode ParserGiD::strToMesherMode(string str) const {
    str = trim(str);
    if (str.compare("Structured")==0) {
        return MeshingParameters::structured;
    } else if (str.compare("Relaxed")==0) {
        return MeshingParameters::relaxed;
    } else if (str.compare("Slanted")==0) {
        return MeshingParameters::slanted;
    } else {
        cerr<< "ERROR @ Parser: ";
        cerr<< "Unreckognized label: " << str<< endl;
        return MeshingParameters::structured;
    }
}

bool
ParserGiD::checkVersionCompatibility(const string version) const {
    bool versionMatches =
            atof(version.c_str()) == atof(string(APP_VERSION).c_str());
    if (!versionMatches) {
        cerr<< "ERROR @ ParserGiD: "
                << "File version " << version << " is not supported." << endl;
    }
    return versionMatches;
}

const ProblemSize* ParserGiD::getProblemSize() const {
    return &pSize_;
}
