/*
 * ParserGiD.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#include "ParserGiD.h"

ParserGiD::ParserGiD() {
}

ParserGiD::ParserGiD(
 const string& fn) {
	string null;
	init(fn, null);
}

ParserGiD::ParserGiD(
 const string& fn,
 const string& pTPath) {
	init(fn, pTPath);
}

ParserGiD::~ParserGiD() {
}

void
ParserGiD::printInfo() const {
	cout << "--- GiDParser info ---" << endl;
	cout << "--- End of GiDParser info ---" << endl;
}

SmbData*
ParserGiD::read() {
	SmbData* res;
	res = new SmbData();

	if (!checkVersionCompatibility(readVersion())) {
		exit(INPUT_ERROR);
	}

	GlobalProblemData* gData = new GlobalProblemData;
	*gData = readProblemData();
	res->gData = gData;
	//res->gData->printInfo();

	pSize = readProblemSize();
	//pSize.printInfo();


	res->mesh = readMesh();
	//res->mesh->printInfo();

	res->pMGroup = readMaterials();
//	res->pMGroup->printInfo();

	res->emSources = readEMSources();
	//res->emSources->printInfo();

	res->outputRequests = readOutputRequests();
	//res->outputRequests->printInfo();

	res->ofParams = readOpenFOAMParameters();
	//res->ofParams->printInfo();

	res->applyGeometricScalingFactor();

	return res;
}

GlobalProblemData
ParserGiD::readProblemData() {
	GlobalProblemData res;
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
					res.finalTime = atof(value.c_str());
				} else if (label.compare("Time step") == 0) {
					res.timeStep = atof(value.c_str());
				} else if (label.compare("Default sampling period") == 0) {
					res.samplingPeriod = atof(value.c_str());
				} else if (label.compare("Geometry scaling factor") == 0) {
					res.scalingFactor = atof(value.c_str());
				} else if (label.compare("Upper x bound") == 0) {
					res.boundTermination[0].second = boundStrToType(value);
				} else if (label.compare("Lower x bound") == 0) {
					res.boundTermination[0].first = boundStrToType(value);
				} else if (label.compare("Upper y bound") == 0) {
					res.boundTermination[1].second = boundStrToType(value);
				} else if (label.compare("Lower y bound") == 0) {
					res.boundTermination[1].first = boundStrToType(value);
				} else if (label.compare("Upper z bound") == 0) {
					res.boundTermination[2].second = boundStrToType(value);
				} else if (label.compare("Lower z bound") == 0) {
					res.boundTermination[2].first = boundStrToType(value);
				} else if (label.compare("Boundary padding") == 0) {
					res.boundaryPadding = atof(value.c_str());
#warning "This has been modified."
				} else if (label.compare("Boundary mesh size") == 0) {
					res.boundaryMeshSize = atof(value.c_str());
#warning "This has been modified."
				} else if (label.compare("Number of processes") == 0) {
					res.numberOfProcesses = atoi(value.c_str());
				} else if (label.compare("Hosts file") == 0) {
					res.hostsFile = trim(value);
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

MeshVolume*
ParserGiD::readMesh() {
	// Read Grid
	RectilinearGrid* grid = readCartesianGrid();
	// Reads the coordinates.
	CoordinateGroup* coordinates = readCoordinates();
	// Reads elements connectivities.
	ElementsGroup elements;
	elements = readElements(*coordinates);
	// Builds mesh with the read data.
	return new MeshVolume(*coordinates, elements, grid);
}

EMSourceGroup*
ParserGiD::readEMSources() {
	vector<Dipole> dipoles;
	vector<PlaneWave> pws;
	vector<Waveport> wps;
	vector<Generator> gen;
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
					Dipole dip = readDipoleEMSource();
					dipoles.push_back(dip);
				} else if (label.compare("Planewave")==0) {
					PlaneWave pw = readPlaneWaveEMSource();
					pws.push_back(pw);
				} else if (label.compare("Generator")==0) {
					gen.push_back(readGeneratorEMSource());
				} else if (label.compare("Waveport")==0) {
					Waveport wp = readWaveportEMSource();
					wps.push_back(wp);
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
	return new EMSourceGroup(dipoles, pws, wps, gen);
}

PhysicalModelGroup*
ParserGiD::readMaterials(){
	vector<const PMPredefined*> predef;
	PMPMC pmpmc;
	PMPEC pmpec;
	PMSMA pmsma;
	vector<PMVolume> vol;
	vector<PMVolumeDispersive> dispVol;
	vector<PMVolumePML*> pmlVol;
	vector<PMSurface*> surf;
	vector<PMWire*> wires;
	vector<PMMultiport*> multiport;
	string label, value;
	unsigned int materialCount = 0;
	bool found = false;
	while (!found && !f_in.eof() ) {
		getNextLabelAndValue(label, value);
		if (label.compare("Materials")==0) {
			found = true;
			while (!f_in.eof() && label.compare("End of materials")!=0) {
				getNextLabelAndValue(label, value);
				if (label.compare("Material")==0) {
					bool materialFinished = false;
					unsigned int id = atoi(value.c_str());
					string name;
					PhysicalModelGroup::Type type
				     = PhysicalModelGroup::undefined;
					PMMultiport::Type multiportType = PMMultiport::undefined;
					double rPermittivity, rPermeability, eCond, mCond;
					double radius, resistance, inductance, capacitance;
					string filename;
					while (!f_in.eof() && !materialFinished) {
						// Reads parameters.
						getNextLabelAndValue(label, value);
						if (label.compare("Name")==0) {
							name = trim(value);
						} else if (label.compare("TypeId")==0) {
							type = materialStrToType(value);
							if (type == PhysicalModelGroup::multiport) {
								multiportType =
								 materialStrToMultiportType(value);
							}
						} else if (label.compare("Permittivity")==0) {
							rPermittivity = atof(value.c_str());
						} else if (label.compare("Permeability")==0) {
							rPermeability = atof(value.c_str());
						} else if (label.compare("Electric Conductivity")==0) {
							eCond = atof(value.c_str());
						} else if (label.compare("Magnetic Conductivity")==0) {
							mCond = atof(value.c_str());
						} else if (label.compare("Radius")==0) {
							radius = atof(value.c_str());
						} else if (label.compare("Resistance")==0) {
							resistance = atof(value.c_str());
						} else if (label.compare("Inductance")==0) {
							inductance = atof(value.c_str());
						} else if (label.compare("Capacitance")==0) {
							capacitance = atof(value.c_str());
						} else if (label.compare("File name")==0) {
							filename = value;
						} else if (label.compare("End of Material")==0) {
							// Creates material.
							switch (type) {
							case PhysicalModelGroup::PEC:
								pmpec = PMPEC(id, name);
								predef.push_back(&pmpec);
								break;
							case PhysicalModelGroup::PMC:
								pmpmc = PMPMC(id, name);
								predef.push_back(&pmpmc);
								break;
							case PhysicalModelGroup::SMA:
								pmsma = PMSMA(id, name);
								predef.push_back(&pmsma);
								break;
							case PhysicalModelGroup::classic:
								if (eCond == 0 && mCond == 0) {
									vol.push_back(
								     PMVolume(id, name,
								      rPermittivity, rPermeability));
								} else {
									dispVol.push_back(
									 PMVolumeDispersive(id, name,
									  rPermittivity, rPermeability,
									  eCond, mCond));
								}
								break;
							case PhysicalModelGroup::elecDispersive:
								dispVol.push_back(
								 readDispersiveMaterialFile(id,name));
								break;
							case PhysicalModelGroup::isotropicsibc:
								PMSurface *aux;
								aux = new PMSurface;
								*aux = readIsotropicSurfaceMaterialFile(id, name);
								aux->printInfo();
								surf.push_back(aux);
								break;
							case PhysicalModelGroup::wire:
								wires.push_back(new PMWire(id, name, radius,
								  resistance, inductance));
								break;
							case PhysicalModelGroup::multiport:
								if (multiportType == PMMultiport::shortCircuit) {
									multiport.push_back(
									 new PMMultiportPredefined(id, name,
									 multiportType));
								} else {
									multiport.push_back(
									 new PMMultiportRLC(id, name, multiportType,
									  resistance, inductance, capacitance));
								}
								break;
							default:
								cerr<< "ERROR @ Parsing materials: ";
								cerr<< "Material type not recognized." << endl;
								break;
							}
							materialFinished = true;
							materialCount++;
						}
					} // Closes material.
 				}
			} // Closes materials section.
		}
	} // Closes problemDataFound while.
	assert(materialCount == pSize.mat);
	// Throws error messages if a problem was detected.
	if (!found) {
		cerr<< "ERROR @ Parsing materials: "
			<< "EoF was reached and label was not found." << endl;
	}
	//
	return new PhysicalModelGroup(predef, vol, dispVol, pmlVol, surf,
	 multiport, wires);
}

OutputRequestGroup*
ParserGiD::readOutputRequests() {
	vector<OutputRequest> oR;
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
					vector<OutputRequest> aux = readOutputRequestInstances();
					for (uint i = 0; i < aux.size(); i++) {
						oR.push_back(aux[i]);
					}
				} else if (label.compare("End of Output Requests")==0) {
					finished = true;
				}
			}
		}
	}
	return (new OutputRequestGroup(oR));
}

vector<OutputRequest>
ParserGiD::readOutputRequestInstances() {
	GiDOutputType gidOutputType = ParserGiD::undefined;
	string line, label, value;
	vector<OutputRequest> res;
	bool finished = false;
	string outputName;
	OutputRequest::Type outputType = OutputRequest::undefined;
	Domain domain;
	vector<uint> elemVec;
	uint nE = 0;
	while (!finished && !f_in.eof()) {
		getNextLabelAndValue(label,value);
		if (label.compare("GiDOutputType")==0) {
			gidOutputType = gidOutputTypeStrToType(trim(value));
		} else if (label.compare("Number of elements")==0) {
			nE = atoi(value.c_str());
			for (unsigned int i = 0; i < nE; i++) {
				getNextLabelAndValue(label,value);
				outputName = trim(value);
				getNextLabelAndValue(label,value);
				outputType = outputTypeStrToType(trim(value));
				getNextLabelAndValue(label,value);
				domain = readDomainFromStr(value);
				switch (gidOutputType) {
				case ParserGiD::outRqOnPoint:
					getNextLabelAndValue(label,value);
					elemVec.clear();
					elemVec.push_back(atoi(value.c_str()));
					res.push_back(OutputRequest(domain, Element::NODE,
							outputType, outputName, elemVec));
					break;
				case ParserGiD::outRqOnLine:
					getNextLabelAndValue(label,value);
					elemVec.clear();
					elemVec.push_back(atoi(value.c_str()));
					res.push_back(OutputRequest(domain, Element::LINE,
							outputType, outputName, elemVec));
					break;
				case ParserGiD::outRqOnSurface:
					getNextLabelAndValue(label,value);
					elemVec.clear();
					elemVec.push_back(atoi(value.c_str()));
					res.push_back(OutputRequest(domain, Element::SURFACE,
							outputType, outputName, elemVec));
					break;
				case ParserGiD::outRqOnVolume:
					getline(f_in, line);
					res.push_back(OutputRequest(domain, Element::VOLUME,
							outputType, outputName,
							BoundingBox(readBoundFromStr(line))));
					break;
				case ParserGiD::farField:
				{
					getline(f_in, line);
					BoundingBox bbox(readBoundFromStr(line));
					double iTh, fTh, sTh, iPhi, fPhi, sPhi;
					f_in >> iTh >> fTh >> sTh >> iPhi >> fPhi >> sPhi;
					getline(f_in, line);
					res.push_back(OutputRequest(domain, Element::VOLUME,
							outputType, outputName, bbox,
							iTh, fTh, sTh, iPhi, fPhi, sPhi));

				}
				break;
				case ParserGiD::undefined:
					cerr<< "ERROR @ GiDParser: "
					<< "Unreckognized GiD Output request type:"
					<< outputType << endl;
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
	return res;
}

OpenFOAMParameters*
ParserGiD::readOpenFOAMParameters() {
	bool finished;
	bool found = false;
	string line, label, value;
	bool castellatedMesh, snapMesh, addLayers;
	uint featureRefLevel;
	double edgeFeatureAngle;
	while (!found && !f_in.eof() ) {
		getNextLabelAndValue(label, value);
		if (label.compare("Openfoam parameters") == 0) {
			found = true;
			finished = false;
			while (!finished && !f_in.eof() ) {
				getNextLabelAndValue(label, value);
				if (label.compare("Castellated mesh") == 0) {
					castellatedMesh = strToBool(value);
				} else if (label.compare("Snap mesh") == 0) {
					snapMesh = strToBool(value);
				} else if (label.compare("Add layers") == 0) {
					addLayers = strToBool(value);
				} else if (label.compare("Edge feature angle") == 0) {
					edgeFeatureAngle = atof(value.c_str());
				} else if (label.compare("Feature refinement level") == 0) {
					featureRefLevel = atoi(value.c_str());
				} else if (label.compare("End of Openfoam parameters")==0) {
					finished = true;
				}
			}
		}
	}
	//
	return new OpenFOAMParameters(castellatedMesh, snapMesh, addLayers,
	 edgeFeatureAngle, featureRefLevel);
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
					cerr<< "ERROR @ Parser::readProblemSize()"   << endl;
					cerr<< "End of problem size label not found" << endl;
					cerr<< "Terminating" << endl;
					exit(INPUT_ERROR);
				}
			}
		}
	}
	// Throws error message if label was not found.
	if (!problemSizeFound) {
		cerr<< "ERROR @ Parser::readProblemSize: " << endl;
		cerr<< "Problem size label not found."     << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	//
	return res;
}

CoordinateGroup*
ParserGiD::readCoordinates() {
	string line;
	unsigned int id;
	CartesianVector<double,3> pos;
	vector<Coordinate<double,3> > coord;
	coord.reserve(pSize.v);
	// Runs over input data file.
	bool finished = false;
	bool found = false;
	while (!found && !f_in.eof() && !finished) {
		getline(f_in, line);
		if (line.find("Coordinates:") != line.npos) {
			found = true;
			// Reads coordinates.
			for (unsigned int i = 0; i < pSize.v; i++) {
				f_in >> id >> pos(0) >> pos(1) >> pos(2);
				coord.push_back(Coordinate<double,3>(id, pos));
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
	// Shows error messages.
	if (!found) {
		cerr<< "ERROR @ GiDParser::readCoordinates()" << endl;
		cerr<< "Coordinates label was not found."  << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	if (!finished) {
		cerr<< "ERROR @ GiDParser::readCoordinates()"       << endl;
		cerr<< "\"End of coordinates\" label not found." << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	//
	return new CoordinateGroup(coord);
}


ElementsGroup
ParserGiD::readElements(const CoordinateGroup& v) {
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
		cerr<< "ERROR@GiDParser::readElements()" << endl;
		cerr<< "\"Elements\" label was not found." << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	// This code is reached only in case of "End of elements" is not found.
	if (!finished) {
		cerr<< "ERROR@GiDParser::readElements()" << endl;
		cerr<< "\"End of elements\" label was not found." << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	//
	ElementsGroup res(lin2, tri3, tri6, tet4, tet10, hex8);
	return res;
}

vector<Hex8>
ParserGiD::readHex8Elements(const CoordinateGroup& v) {
	vector<Hex8> res;
	unsigned int id, matId, vId[8];
	res.reserve(pSize.hex8);
	for (unsigned int i = 0; i < pSize.hex8; i++) {
		f_in >> id;
		for (unsigned int j = 0; j < 8; j++) {
			 f_in >> vId[j];
		}
		f_in >> matId;
		res.push_back(Hex8(v, id, matId, vId));
	}
	return res;
}

vector<Tet10>
ParserGiD::readTet10Elements(const CoordinateGroup& v) {
	vector<Tet10> res;
	unsigned int id, matId, vId[10];
	res.reserve(pSize.tet10);
	for (unsigned int i = 0; i < pSize.tet10; i++) {
		f_in >> id;
		for (unsigned int j = 0; j < 10; j++) {
			 f_in >> vId[j];
		}
		f_in >> matId;
		res.push_back(Tet10(v, id, matId, vId));
	}
	return res;
}

vector<Tet4>
ParserGiD::readTet4Elements(
 const CoordinateGroup& v) {
	vector<Tet4> res;
	unsigned int id, matId, vId[4];
	res.reserve(pSize.tet4);
	for (unsigned int i = 0; i < pSize.tet4; i++) {
		f_in >> id >> vId[0] >> vId[1] >> vId[2] >> vId[3] >> matId;
		res.push_back(Tet4(v, id, matId, vId));
	}
	return res;
}

vector<Tri6>
ParserGiD::readTri6Elements(const CoordinateGroup& v) {
	vector<Tri6> res;
	unsigned int id, matId, vId[6];
	CartesianVector<double,3> normal;
	res.reserve(pSize.tri6);
	for (unsigned int i = 0; i < pSize.tri6; i++) {
		f_in >> id;
		for (unsigned int j = 0; j < 6; j++)
			 f_in >> vId[j];
		f_in >> matId >> normal(0) >> normal(1) >> normal(2);
		res.push_back(Tri6(v, id, matId, vId, normal));
	}
	return res;
}


vector<Tri3>
ParserGiD::readTri3Elements(const CoordinateGroup& v) {
	vector<Tri3> res;
	unsigned int id, matId, vId[3];
	CartesianVector<double,3> normal;
	res.reserve(pSize.tri3);
	for (unsigned int i = 0; i < pSize.tri3; i++) {
		f_in >> id >> vId[0] >> vId[1] >> vId[2] >>
		 matId >> normal(0) >> normal(1) >> normal(2);
		res.push_back(Tri3(v, id, matId, vId, normal));
	}
	return res;
}

vector<Lin2>
ParserGiD::readLin2Elements(const CoordinateGroup& v) {
	vector<Lin2> res;
	unsigned int id, matId, vId[2];
	res.reserve(pSize.lin2);
	for (unsigned int i = 0; i < pSize.lin2; i++) {
		f_in >> id >> vId[0] >> vId[1] >> matId;
		res.push_back(Lin2(v, id, matId, vId));
	}
	return res;
}

PMVolumeDispersive
ParserGiD::readDispersiveMaterialFile(
 const unsigned int id_, const string& fileName) const {
	ifstream matFile;
	string matFileName, line, label, value;
	string name, model;
	string poles, epsilon, sigma;
	unsigned int nPoles, nDrudePoles;
	double eps, sig;
	vector<int> poleId, drudePoleId;
	vector<double> rePK, imPK, reRK, imRK;
	vector<double> reDrudePK, imDrudePK, reDrudeRK, imDrudeRK;
	int tmpPoleId;
	double tmpRePK, tmpImPK, tmpReRK, tmpImRK;
	// Opens file, read only mode.
	matFileName = problemTypePath + "/material/" + fileName + ".dat";
	matFile.open(matFileName.c_str(), ifstream::in);
	if (matFile.fail()) {
		cerr<< "ERROR @ readDispersiveMaterialFile()" << endl;
		cerr<< "Problem opening file: " << matFileName << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
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
	for (unsigned int i = 0; i < nPoles; i++) {
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
	for (unsigned int i = 0; i < nDrudePoles; i++) {
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
		for (unsigned int i = 0; i < nPoles; i++) {
			complex<double> pole(rePK[i], imPK[i]);
			poles.push_back(pole);
			complex<double> residue(reRK[i]/2.0, imRK[i]/2.0);
			residues.push_back(residue);
		}
		for (unsigned int i = 0; i < nDrudePoles; i++) {
			complex<double> pole(reDrudePK[i], imDrudePK[i]);
			drudePoles.push_back(pole);
			complex<double> residue(reDrudeRK[i]/2.0, imDrudeRK[i]/2.0);
			drudeResidues.push_back(residue);
		}
		return PMVolumeDispersive(id_, name, eps, 1.0, sig,
		 poles, residues, drudePoles, drudeResidues);
	}
	cerr<< "ERROR@GiDParser::readDispersiveMaterialFile(...)" << endl;
	cerr<< "File contains unknown model." << endl;
	exit(INPUT_ERROR);
}

PMSurface
ParserGiD::readIsotropicSurfaceMaterialFile(
 const int id_, const string& fileName) const {
	ifstream matFile;
	string matFileName, line, label, value;
	string name, model;
	char *pEnd;
	double Zstatic[4], Zinfinite[4];
	vector<double> pole, Z11, Z12, Z21, Z22;
	double tmpP, tmpZ11, tmpZ12, tmpZ21, tmpZ22;
	// Opens file, read only mode.
	matFileName = problemTypePath + "/panel/" + fileName + ".dat";
	matFile.open(matFileName.c_str(), ifstream::in);
	if (matFile.fail()) {
		cerr<< "ERROR @ readSurfaceMaterialFile()" << endl;
		cerr<< "Problem opening file: " << matFileName << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	// Parses first line, containing material name.
	getline(matFile, line);
	if (line.find("#PANEL#") == string::npos) {
		cerr<< "ERROR @ Parser::readSurfaceMaterialFile(...)"   << endl;
		cerr<< "File: " << matFileName                          << endl;
		cerr<< "#PANEL# label has not been found in first line" << endl;
		cerr<< "Terminating"                                    << endl;
		exit(INPUT_ERROR);
	}
	name = line.substr(8, line.length()-9);
	getline(matFile, line);
	unsigned int nPoles = 0;
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
		Zinfinite[0] = strtod(value.c_str(), &pEnd);
		Zinfinite[1] = strtod(pEnd, &pEnd);
		Zinfinite[2] = strtod(pEnd, &pEnd);
		Zinfinite[3] = strtod(pEnd, &pEnd);
	}
	// Gets Zstatic.
	getline(matFile, line);
	label = line.substr(0, line.find(":"));
	if(!label.compare("Zstatic")) {
		value  = line.substr(line.find(":")+2, line.length());
		Zstatic[0] = strtod(value.c_str(), &pEnd);
		Zstatic[1] = strtod(pEnd, &pEnd);
		Zstatic[2] = strtod(pEnd, &pEnd);
		Zstatic[3] = strtod(pEnd, &pEnd);
	}
	// Parses poles.
	// Stores in line the file line containing headers.
	getline(matFile, line);
	for (unsigned int i = 0; i < nPoles; i++) {
		matFile >> tmpP >> tmpZ11 >> tmpZ12 >> tmpZ21 >> tmpZ22;
		pole.push_back(tmpP);
		Z11.push_back(tmpZ11);
		Z12.push_back(tmpZ12);
		Z21.push_back(tmpZ21);
		Z22.push_back(tmpZ22);
	}
	// Copies all parsed data into the aux material depending on the model.
	PMSurface
	 res(id_, name, Zinfinite, Zstatic, pole, Z11, Z12, Z21, Z22);
	return res;
}

void
ParserGiD::getNextLabelAndValue(string& label, string& value) {
	string line;
	getline(f_in, line);
	label = line.substr(0, line.find(LABEL_ENDING));
	value = line.substr(line.find(LABEL_ENDING) + 1, line.length());
}

RectilinearGrid*
ParserGiD::readCartesianGrid() {
	string label, line, value;
	bool finished = false;
	bool gridLabelFound = false;
	bool gridFound = false;
	RectilinearGrid* grid = NULL;
	BoundingBox bound;
	bool sizesByNumberOfCells = true;
	CartesianVector<long,3> numElems;
	while (!gridLabelFound && !f_in.eof()) {
		getline(f_in, line);
		if (line.find("Grid:") != line.npos ) {
			gridLabelFound = true;
			while(!finished) {
				getNextLabelAndValue(label, value);
				if (label.compare("Layer Box")==0) {
					gridFound = true;
					bound = BoundingBox(readBoundFromStr(value));
				} else if (label.compare("Type")==0) {
					if (trim(value).compare("by_number_of_cells")==0) {
						sizesByNumberOfCells = true;
					} else {
						sizesByNumberOfCells = false;
					}
				} else if (label.compare("Directions")==0) {
					CVecD3 aux = strToCartesianVector(value);
					if (sizesByNumberOfCells) {
						numElems(0) = (long) aux(0);
						numElems(1) = (long) aux(1);
						numElems(2) = (long) aux(2);
					} else {
						numElems(0) = (long) (bound.getLength()(0) / aux(0));
						numElems(1) = (long) (bound.getLength()(1) / aux(1));
						numElems(2) = (long) (bound.getLength()(2) / aux(2));
					}
				} else if(label.find("End of Grid") != label.npos) {
					finished = true;
					if (!gridFound) {
						return NULL;
					}
				}
				if (f_in.eof()) {
					cerr<< "ERROR @ ParserGiD()"   << endl;
					cerr<< "End of grid label not found" << endl;
					exit(INPUT_ERROR);
				}
			}
		}
	}
	// Throws error message if label was not found.
	if (!gridLabelFound) {
		cerr<< "ERROR @ ParserGiD: " << endl;
		cerr<< "Grid label not found."     << endl;
		exit(INPUT_ERROR);
	}
	if (gridFound) {
		grid = new RectilinearGrid(bound, numElems.val);
	} else {
		grid = NULL;
	}
	return grid;
}

PlaneWave
ParserGiD::readPlaneWaveEMSource() {
	CartesianVector<double,3> waveDirection, polarization;
	double spread = 0.0;
	double delay = 0.0;
	string filename;
	string label, value;
	pair<CVecD3,CVecD3> bound;
	vector<uint> elems;
	bool isDefinedOnLayer = true;
	while(!f_in.eof()) {
		getNextLabelAndValue(label, value);
		if (label.compare("Direction")==0) {
			waveDirection = strToCartesianVector(value);
		} else if (label.compare("Polarization")==0) {
			polarization = strToCartesianVector(value);
		} else if (label.compare("Gaussian spread")==0) {
			spread = atof(value.c_str());
		} else if (label.compare("Gaussian delay")==0) {
			delay = atof(value.c_str());
		} else if (label.compare("Filename")==0) {
			filename = trim(value);
		} else if (label.compare("Layer Box")==0) {
			bound = readBoundFromStr(value);
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
				return PlaneWave(
				 bound,	 waveDirection, polarization, spread, delay, filename);
			} else {
				return PlaneWave(
				 elems, waveDirection, polarization, spread, delay, filename);
			}
		}
	}
	// Throws error message if ending label was not found.
	cerr<< "ERROR @ Parsing planewave:"
	    << "End of Planewave label not found. " << endl;
	exit(INPUT_ERROR);
}

Dipole
ParserGiD::readDipoleEMSource() {
	vector<unsigned int> elem;
	double length = 0.0;
	CartesianVector<double,3> orientation;
	CartesianVector<double,3> position;
	bool sinModulation = false, gaussModulation = false;
	double sinAmplitude = 0.0, frequency = 0.0,
	 gaussAmplitude = 0.0, spread = 0.0, delay = 0.0;
	//
	string line;
	bool finished = false;
	char* pEnd;
	while(!finished && !f_in.eof()) {
		getline(f_in, line);
		if (line.find("End of puntual excitation") == line.npos) {
			elem.push_back(strtol(line.c_str(), &pEnd, 10 ));
			if (elem.size() == 1) {
				if (strtol(pEnd, &pEnd, 10) == 1) {
					// Ignores soft/hard sources.
				}
				length = strtod(pEnd, &pEnd);
				orientation(0) = strtod(pEnd, &pEnd);
				orientation(1) = strtod(pEnd, &pEnd);
				orientation(2) = strtod(pEnd, &pEnd);
				// Reads sinusoidal modulation parameters.
				if (strtol(pEnd, &pEnd, 10) == 1)
					sinModulation = true;
				else
					sinModulation = false;
				sinAmplitude = strtod(pEnd, &pEnd);
				frequency = strtod(pEnd, &pEnd);
				// Reads gaussian modulation parameters.
				if (strtol(pEnd, &pEnd, 10) == 1) {
					gaussModulation = true;
				} else {
					gaussModulation = false;
				}
				gaussAmplitude = strtod(pEnd, &pEnd);
				spread = strtod(pEnd, &pEnd);
				delay = strtod(pEnd, &pEnd);
			}
		} else
			finished = true;
	}
	// Throws error message if finished was not updated.
	if (!finished) {
		cerr<< "ERROR @ GiDParser::readDipoleEMSource" << endl;
		cerr<< "End of excitation type label not found. " << endl;
		cerr<< "Terminating" << endl;
		exit(INPUT_ERROR);
	}
	//
	Dipole res(elem, length, orientation, position,
	 sinModulation, sinAmplitude, frequency,
	 gaussModulation, gaussAmplitude, spread, delay);
	return res;
}

Waveport
ParserGiD::readWaveportEMSource() {
	vector<unsigned int> elem;
	unsigned int numElements = 0;
	bool input = true;
	double spread = 0.0;
	double delay = 0.0;
	Waveport::Shape shape = Waveport::rectangular;
	Waveport::ExcitationMode excitationMode = Waveport::TE;
	pair<unsigned int,unsigned int> mode(1,0);
	Waveport::Symmetry symXY = Waveport::none;
	Waveport::Symmetry symYZ = Waveport::none;
	Waveport::Symmetry symZX = Waveport::none;
	string line, label, value;
	bool finished = false;
	while (!finished && !f_in.eof()) {
		getline(f_in, line);
		label = line.substr(0, line.find(LABEL_ENDING));
		value = line.substr(line.find(LABEL_ENDING)+1, line.length());
		if (!label.compare("Input")) {
			unsigned int oneOrZero;
			oneOrZero = atoi(value.c_str());
			if (oneOrZero == 1) {
				input = true;
			} else {
				input = false;
			}
		} else if (!label.compare("Spread")) {
			spread = atof(value.c_str());
		} else if (!label.compare("Delay")) {
			delay = atof(value.c_str());
		} else if (!label.compare("Shape")) {
			if (value.find("Rectangular") != value.npos) {
				shape = Waveport::rectangular;
			} else {
				cout << "ERROR @ Unreckognized waveport shape." << endl;
				exit(-1);
			}
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
		} else if (!label.compare("Symmetry_XY")) {
			if (value.find("Undefined") != value.npos) {
				symXY = Waveport::none;
			} else if (value.find("PMC") != value.npos) {
				symXY = Waveport::pmc;
			} else if (value.find("PEC") != value.npos) {
				symXY = Waveport::pec;
			}
		} else if (!label.compare("Symmetry_YZ")) {
			if (value.find("Undefined") != value.npos) {
				symYZ = Waveport::none;
			} else if (value.find("PMC") != value.npos) {
				symYZ = Waveport::pmc;
			} else if (value.find("PEC") != value.npos) {
				symYZ = Waveport::pec;
			}
		} else if (!label.compare("Symmetry_ZX")) {
			if (value.find("Undefined") != value.npos) {
				symZX = Waveport::none;
			} else if (value.find("PMC") != value.npos) {
				symZX = Waveport::pmc;
			} else if (value.find("PEC") != value.npos) {
				symZX = Waveport::pec;
			}
		} else if (!label.compare("Number of elements")) {
			numElements = atoi(value.c_str());
		} else if (!label.compare("Elements")) {
			unsigned int e;
			for (unsigned int i = 0; i < numElements; i++) {
				f_in >> e;
				elem.push_back(e);
			}
		} else if (label.find("End of Waveport") != label.npos) {
			finished = true;
		}
		if (f_in.eof()) {
			cerr<< "ERROR @ Parser"   << endl;
			cerr<< "End of Waveport not found" << endl;
			cerr<< "Terminating" << endl;
			exit(INPUT_ERROR);
		}
	}
	// Throws error message if finished was not updated.
	if (!finished) {
		cerr<< "ERROR @ GiDParser::readWaveportEMSource" << endl;
		cerr<< "End of excitation type label not found. " << endl;
		exit(INPUT_ERROR);
	}
	//
	Waveport res(elem, input, spread, delay,
	 shape, excitationMode, mode, symXY, symYZ, symZX);
	return res;
}

Generator
ParserGiD::readGeneratorEMSource() {
	double spread = 0.0;
	double delay = 0.0;
	Generator::Type type;
	vector<uint> elems;
	string filename;
	string label, value;
	while(!f_in.eof()) {
 		getNextLabelAndValue(label, value);
		if (label.compare("Type")==0) {
			type = generatorStrToType(value);
		} else if (label.compare("Gaussian spread")==0) {
			spread = atof(value.c_str());
		} else if (label.compare("Gaussian delay")==0) {
			delay = atof(value.c_str());
		} else if (label.compare("Filename")==0) {
			filename = trim(value);
		} else if (label.compare("Number of elements")==0) {
			uint nE = atoi(value.c_str());
			elems.reserve(nE);
			for (uint i = 0; i < nE; i++) {
				uint e;
				f_in >> e;
				elems.push_back(e);
			}
		} else if (label.compare("End of Generator")==0) {
			return Generator(type, elems, spread, delay, filename);
		}
	}
	// Throws error message if ending label was not found.
	cerr<< "ERROR @ Parsing generator: "
	    << "End of Generator label not found. " << endl;
	exit(INPUT_ERROR);
}

Element::Type
ParserGiD::elementTypeStrToType(string str) const {
	str = trim(str);
	if (str.compare("point")==0) {
		return Element::NODE;
	} else if (str.compare("line")==0) {
		return Element::LINE;
	} else if (str.compare("surface")==0) {
		return Element::SURFACE;
	} else if (str.compare("volume")==0) {
		return Element::VOLUME;
	} else {
		cerr<< "ERROR @ GiDParser::readOutputRequestInstance(): "
			<< "Unreckognized element type: " << str << endl;
		return Element::undefined;
	}
}

OutputRequest::Type
ParserGiD::outputTypeStrToType(string str) const {
	str = trim(str);
	if (str.compare("electricField")==0) {
		return OutputRequest::electricField;
	} else if (str.compare("magneticField")==0) {
		return OutputRequest::magneticField;
	} else if (str.compare("electricFieldNormals")==0) {
		return OutputRequest::electricFieldNormals;
	} else if (str.compare("magneticFieldNormals")==0) {
		return OutputRequest::magneticFieldNormals;
//	} else if (str.compare("powerDensity")==0) {
//		return OutputRequest::powerDensity;
//	} else if (str.compare("power")==0) {
//		return OutputRequest::power;
	} else if (str.compare("current")==0) {
		return OutputRequest::current;
	} else if (str.compare("voltage")==0) {
		return OutputRequest::voltage;
	} else if (str.compare("bulkCurrentElectric")==0) {
		return OutputRequest::bulkCurrentElectric;
	} else if (str.compare("bulkCurrentMagnetic")==0) {
		return OutputRequest::bulkCurrentMagnetic;
	} else if (str.compare("farField")==0) {
		return OutputRequest::farField;
	} else {
		cerr<< "ERROR @ GiDParser::readOutputRequestInstance(): "
			<< "Unrecognized output type: " << str << endl;
		return OutputRequest::undefined;
	}
}

Generator::Type
ParserGiD::generatorStrToType(string str) const {
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

GlobalProblemData::boundType
ParserGiD::boundStrToType(string str) const {
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

PhysicalModelGroup::Type
ParserGiD::materialStrToType(string str) const {
	str = trim(str);
	if (str.compare("PEC")==0) {
		return PhysicalModelGroup::PEC;
	} else if (str.compare("PMC")==0) {
		return PhysicalModelGroup::PMC;
	} else if (str.compare("PML")==0) {
		return PhysicalModelGroup::PML;
	} else if (str.compare("SMA")==0) {
		return PhysicalModelGroup::SMA;
	} else if (str.compare("Classic")==0) {
		return PhysicalModelGroup::classic;
	} else if (str.compare("Dispersive")==0) {
		return PhysicalModelGroup::elecDispersive;
	} else if (str.compare("SIBC")==0) {
		return PhysicalModelGroup::isotropicsibc;
	} else if (str.compare("Wire")==0) {
		return PhysicalModelGroup::wire;
	} else if (str.find("Conn_") != string::npos) {
		return PhysicalModelGroup::multiport;
	} else {
		cerr<< "ERROR @ Parser: "
			<< "Unreckognized material label." << endl;
		return PhysicalModelGroup::undefined;
	}
}

PMMultiport::Type
ParserGiD::materialStrToMultiportType(string str) const {
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
		cerr<< "ERROR @ Parser" << endl;
		cerr<< "Unreckognized multiport label." << endl;
		exit(INPUT_ERROR);
	}
}

pair<CVecD3, CVecD3>
ParserGiD::readBoundFromStr(
 const string& value) const {
	unsigned int begin = value.find_first_of("{");
	unsigned int end = value.find_last_of("}");
	istringstream iss(value.substr(begin+1,end-2));
	string sub;
	CartesianVector<double,3> max, min;
	for (unsigned int i = 0; i < 3; i++) {
		iss >> sub;
		max(i) = atof(sub.c_str());
	}
	for (unsigned int i = 0; i < 3; i++) {
		iss >> sub;
		min(i) = atof(sub.c_str());
	}
	pair<CVecD3,CVecD3> bound(min, max);
	return bound;
}



void
ParserGiD::init(const string& fn, const string& pTPath) {
	// Check if fn is valid.
	struct stat st;
	if (stat(fn.c_str(), &st) == 0) {
		if (st.st_mode & S_IFDIR) {
			cerr<< "ERROR@GiDParser::GiDParser()" << endl;
			cerr<< "Problem opening: " << endl;
			cerr<< fn << endl;
			cerr<< "It is a directory " << endl;
			exit(INPUT_ERROR);
		}  else if(st.st_mode & S_IFREG) {
			f_in.open(fn.c_str(), ifstream::in);
			problemTypePath = pTPath;
			if (f_in.fail()) {
				cerr<< "ERROR @ ParserGiD::GiDParser()" << endl;
				cerr<< "Problem opening file: " << endl;
				cerr<< fn << endl;
				exit(INPUT_ERROR);
			}
			return;
		}
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
ParserGiD::gidOutputTypeStrToType(string str) const {
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
ParserGiD::readDomainFromStr(const string& line) const {
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

bool
ParserGiD::checkVersionCompatibility(const string version) const {
	bool versionMatches = atof(version.c_str()) == atof(APP_VERSION);
	if (!versionMatches) {
		cerr<< "ERROR @ ParserGiD: "
			<< "File version " << version << " is not supported." << endl;
	}
	return versionMatches;
}
