/*
 * RCSResult.cpp
 *
 *  Created on: Nov 2, 2012
 *      Author: luis
 */
#ifndef RESULTRCS_H_
#include "ResultRCS.h"
#endif

ResultRCS::ResultRCS(
 const string& filename,
 const string& resultName,
 const MeshVolume& meshVol,
 const vector<const BoundaryCondition*>& iB)
  : ProjectFile(filename) {
	uint bSize = iB.size();
	vector<pair<const Volume*, uint> > cellsBorder;
	pair<const Volume*, uint> inner, outer;
	pair<uint, uint> elFace;
	for (uint i = 0; i < bSize; i++) {
		inner = iB[i]->get();
		outer = meshVol.getNeighConnection(inner);
		cellsBorder.push_back(outer);
		uint id = outer.first->getId();
		elFace.first = cells.getGlobalRelPosOfId(id);
		elFace.second = outer.second;
		elemFace.push_back(elFace);
	}
	// Opens file, writes header.
	numNodes = nfp * iB.size();
	writeHeader(cellsBorder);
}

ResultRCS::ResultRCS(
 const string& filename,
 const Symmetry::Symmetry sym[3]) {
	numNodes = 0;
	nElem = 0;
	// Opens file, read only mode.
	file.open(filename.c_str(), ifstream::in);
	// Checks errors when opening.
	if (file.fail()) {
		cerr << endl << "ERROR@ResultRCS ctor"    << endl;
		cerr << endl << "Problem opening file: " << endl;
		cerr << filename << endl;
		exit(-1);
	}
	// Reads file.
	readHeader();
	readFields();
	// Apply simmetries.
	applySymmetries(sym);
}

ResultRCS::~ResultRCS() {
	file.close();
}

vector<CartesianVector<complex<double>,3> >
ResultRCS::nearToFarField(
 const double frequency,
 const vector<pair<double, double> >& directions) const {
	assert(numNodes > 0);
	assert(stepsGroup.step.size() > 0);
	const uint nDir = directions.size();
	// Performs Fourier Transform of fields at selected frequency.
	vector<double> time = stepsGroup.getTimeVector();
 	// Computes contributions of each cell.
 	CartesianVector<complex<double>,3> **ERad;
 	ERad = new CartesianVector<complex<double>,3>*[nElem];
 	for (uint e = 0; e < nElem; e++) {
 		ERad[e] = new CartesianVector<complex<double>,3>[nDir];
 	}
 	uint e;
#ifdef USE_OPENMP
#pragma omp parallel for private(e)
#endif
	for (e = 0; e < nElem; e++) {
		CellTri<ORDER_N> *surf;
		if (!quadraticMesh) {
			const uint nbp = 3;
			const CoordD3* coord[nbp];
			for (uint i = 0; i < nbp; i++) {
				coord[i] = &vertex[nbp * e + i];
			}
			Tri3 base(coord);
			surf = new CellTri3<ORDER_N>(base);
		} else {
			const uint nbp = 6;
			const CoordD3* coord[nbp];
			for (uint i = 0; i < nbp; i++) {
				coord[i] = &vertex[nbp * e + i];
			}
			Tri6 base(coord);
			surf = new CellTri6<ORDER_N>(base);
		}
		// Obtains transformation in nodes.
		CartesianVector<complex<double>,3> elecFq[nfp], magnFq[nfp];
		vector<CVecD3 > electric(stepsGroup.nSteps);
		vector<CVecD3 > magnetic(stepsGroup.nSteps);
		for (uint node = 0; node < nfp; node++) {
			electric = stepsGroup.getElectricField(node + e*nfp);
			elecFq[node] =
			 MathUtils::getDFT(frequency, time, electric);
			magnetic = stepsGroup.getMagneticField(node + e*nfp);
			magnFq[node] =
			 MathUtils::getDFT(frequency, time, magnetic);
		}
		for (uint i = 0; i < nDir; i++) {
			ERad[e][i] +=
			 surf->getRadiatedField(elecFq, magnFq, frequency, directions[i]);
		}
		delete surf;
	}
	// Prepares result.
	vector<CartesianVector<complex<double>,3> > res(nDir);
	for (uint i = 0; i < nDir; i++) {
		for (uint e = 0; e < nElem; e++) {
			res[i] += ERad[e][i];
		}
	}
	return res;
}

vector<double>
ResultRCS::getRCS(
 const double frequency,
 const vector<pair<double, double> >& directions) const {
	const double betaSq = pow(2*M_PI*frequency/SPEED_OF_LIGHT,2);
	// Incident Field.
	CartesianVector<complex<double>,3> EIncFq;
	vector<CVecD3 > EInc;
	EInc = stepsGroup.getIncidentField();
	EIncFq = MathUtils::getDFT(frequency, stepsGroup.getTimeVector(), EInc);
	const double EIncSq = pow(EIncFq.norm(),2);
	// Radiated Field.
 	vector<CartesianVector<complex<double>,3> > ERadFq;
 	ERadFq = nearToFarField(frequency, directions);
 	vector<double> res(directions.size());
 	for (uint i = 0; i < directions.size(); i++) {
 		double ERadSq = pow(ERadFq[i].norm(),2);
 		res[i] = ERadSq * betaSq / EIncSq / (4.0 * M_PI);
 	}
 	return res;
}

double
ResultRCS::getRCS(
 const double frequency,
 const pair<double, double>& directions) const {
	vector<double> res;
	vector<pair<double,double> > dir;
	dir.push_back(directions);
	res = getRCS(frequency, dir);
 	return res[0];
}

void
ResultRCS::writeResult(
 const FieldR3& elec,
 const FieldR3& magn,
 const CVecD3& EInc,
 const double time) {
	uint i, e, f, j, k;
	static const SimplexTet<N> tet;
	// RCS step information.
	file << "RCSSTEP: " << resultStep++ << endl;
	file << "time: " << time << endl;
	// Writes incident field.
	file << "ExInc EyInc EzInc:" << endl;
	file << EInc(x) << " " << EInc(y) << " " << EInc(z) << endl;
	// Writes electric field.
	file << "Ex Ey Ez:" << endl;
	for (i = 0; i < elemFace.size(); i++) {
		e = elemFace[i].first;
		f = elemFace[i].second;
		for (j = 0; j < nfp; j++) {
			k = e * np + tet.sideNode(f, j);
			file << elec(x)[k] << " " << elec(y)[k] << " " << elec(z)[k] << endl;
		}
	}
	// Writes magnetic field.
	file << "Hx Hy Hz:" << endl;
	for (i = 0; i < elemFace.size(); i++) {
		e = elemFace[i].first;
		f = elemFace[i].second;
		for (j = 0; j < nfp; j++) {
			k = e * np + tet.sideNode(f, j);
			file << magn(x)[k] << " " << magn(y)[k] << " " << magn(z)[k] << endl;
		}
	}
	// Writes ending of RCS step.
	file << "END RCSSTEP" << endl << endl;
}

void
ResultRCS::printInfo() const {
	cout << " --- RCSResult info --- " << endl;
	cout << "Current resultStep: " << resultStep << endl;
	cout << "# Nodes: " << numNodes << endl;
	cout << "Elem-face pairs: " << elemFace.size() << endl;
}
void
ResultRCS::readHeader() {
	string line, label, value;
	bool finished = false;
	bool headerFound = false;
	while (!headerFound && !file.eof()) {
		getline(file, line);
		if (line.find("HEADER") != line.npos) {
			headerFound = true;
			while (!finished && !file.eof() ) {
				getline(file, line);
				label = line.substr(0, line.find(LABEL_ENDING));
				value = line.substr(line.find(LABEL_ENDING)+1, line.length());
				if (!label.compare("N")) {
					uint order = atoi(value.c_str());
					if (order != N) {
						cout << "Incompatible order." << endl;
						exit(-1);
					}
				} else if (!label.compare("Nm")) {
					numNodes = atoi(value.c_str());
					nElem = numNodes / nfp;
				} else if (label.find("nx ny nz") != label.npos) {
//					normal.reserve(nElem);
					CVecD3 aux;
					for (uint i = 0; i < numNodes; i++) {
						file  >> aux(0) >> aux(1) >> aux(2);
//						// Stores only one for element.
//						if (i % nfp == 0) {
//							normal.push_back(aux);
//						}
					}
				} else if (label.find("x y z") != label.npos) {
//					nodes.reserve(numNodes);
					CVecD3 aux;
					for (uint i = 0; i < numNodes; i++) {
						file  >> aux(0) >> aux(1) >> aux(2);
						// Stores only one for element.
//						if (i % nfp == 0) {
//							nodes.push_back(aux);
//						}
					}
				}  else if (label.find("vx vy vz") != label.npos) {
					char* pEnd;
					// Size of vertex will match numNodes only if N = Nb.
					vertex.reserve(numNodes);
					uint vertexCount = 1;
					CVecD3 aux;
					while (finished == false && !file.eof()) {
						getline(file, line);
						if (line.find("END HEADER") != line.npos) {
							finished = true;
						} else {
							aux(0) = strtod(line.c_str(), &pEnd);
							aux(1) = strtod(pEnd, &pEnd);
							aux(2) = strtod(pEnd, &pEnd);
							CoordD3
							 auxCoord(vertexCount++, aux);
							vertex.push_back(auxCoord);
						}
					}
					if ((vertex.size() / nElem) == 3) {
						quadraticMesh = false;
					} else if ((vertex.size() / nElem) == 6) {
						quadraticMesh = true;
					} else {
						cout << "ERROR @ Parser" << endl;
						cout << "Incompatible number of vertices." << endl;
						exit(-1);
					}
				}
			} // Closes ( !finished && !file.eof() ) while.
		} // Closes problem data found if.
	} // Closes headerFound while.
	// Throws error messages if a problem was detected.
	if (!headerFound) {
		cerr << endl << "ERROR @ Parser::readProblemData()" << endl;
		cerr << endl << "EoF was reached but problem data was not found." << endl;
		cerr << endl << "Terminating." << endl;
		exit(-1);
	}
	if (!finished) {
		cerr << endl << "ERROR @ GiDParser::readProblemData()" << endl;
		cerr << endl << "EoF reached, \"end of problem data\" not found." << endl;
		cerr << endl << "Terminating." << endl;
		exit(-1);
	}
}

void
ResultRCS::writeHeader(
 const vector<pair<const Volume*, uint> >& border) {
	file << "RCS analysis results file" << endl;
	file << "HEADER" << endl;
	file << "N: " << ORDER_N << endl;
	file << "NODETOL: " << CELL_NODE_TOLERANCE << endl;
	file << "Nm: " << numNodes << endl;
	// Writes normal vectors.
	file << "nx ny nz:" << endl;
	for (uint i = 0; i < border.size(); i++) {
		const Volume* vol = border[i].first;
		uint f = border[i].second;
		for (uint j = 0; j < nfp; j++) {
			CVecD3 nor = vol->sideNormal(f);
			file << - nor(0) << " " << - nor(1) << " " << - nor(2) << endl;
		}
	}
	// Writes node coordinates.
	file << "vx vy vz:" << endl;
	for (uint i = 0; i < border.size(); i++) {
		const Volume* vol = border[i].first;
		uint f = border[i].second;
		for (uint j = 0; j < nfp; j++) {
			CVecD3 pos = vol->getSideV(f,j)->pos();
			file << pos(0) << " " << pos(1) << " " << pos(2) << endl;
		}
	}
	file << "END HEADER"                                         << endl;
}
void
ResultRCS::readFields() {
	assert(numNodes != 0);
	stepsGroup.step.reserve(estimatedNumberOfSteps);
	stepsGroup.nSteps = 0;
	string line, label, value;
	int num;
	double time;
	bool ok = true;
	CVecD3 incident, aux;
	vector<CVecD3 >
	 electric(numNodes), magnetic(numNodes);
	while (!file.eof() && ok) {
		getline(file, line);
		label = line.substr(0, line.find(LABEL_ENDING));
		value = line.substr(line.find(LABEL_ENDING)+1, line.length());
		if (!label.compare("RCSSTEP")) {
			num = atoi(value.c_str());
		} else if (!label.compare("time")) {
			time = atof(value.c_str());
		} else if (!label.compare("ExInc EyInc EzInc")) {
			file >> incident(0) >> incident(1) >> incident(2);
		} else if (!label.compare("Ex Ey Ez")) {
			for (uint i = 0; i < numNodes; i++) {
				file >> aux(0) >> aux(1) >> aux(2);
				electric[i] = aux;
			}
		} else if (!label.compare("Hx Hy Hz")) {
			for (uint i = 0; i < numNodes; i++) {
				file >> aux(0) >> aux(1) >> aux(2);
				magnetic[i] = aux;
			}
			getline(file,line);
			getline(file,line);
			if (!line.compare("END RCSSTEP")) {
				stepsGroup.nSteps++;
				stepsGroup.step.push_back(
				 new RCSStep_s(num, time, incident, electric, magnetic)
				);
			} else {
				ok = false;
				break;
			}
		}
	}
	if (!ok) {
		cout << "ERROR @ Reading RCS Step." << endl;
		exit(-1);
	}
}

double
ResultRCS::getSamplingTime() const {
	// Performs Fourier Transform of fields at selected frequency.
	double samplingTime;
	vector<double> time = stepsGroup.getTimeVector();
	samplingTime = MathUtils::meanDifference(time);
	return samplingTime;
}

void
ResultRCS::applySymmetries(
 const Symmetry::Symmetry sym[3]) {
	if (sym[0] != Symmetry::none) {
		nElem *= 2;
		numNodes *= 2;
		uint newVertexSize = 2 * vertex.size();
		vertex.reserve(newVertexSize);
		uint vertexCounter = newVertexSize / 2;
		for (uint i = 0; i < newVertexSize / 2; i++) {
			CVecD3 auxVec;
			auxVec(0) =  vertex[i](0);
			auxVec(1) =  vertex[i](1);
			auxVec(2) = -vertex[i](2);
			CoordD3 aux(vertexCounter++,auxVec);
			vertex.push_back(aux);
		}
		if (sym[0] == Symmetry::pec) {
			cout << "Not Done" << endl;
			exit(-1);
		}
		if (sym[0] == Symmetry::pmc) {
			for (uint s = 0; s < stepsGroup.step.size(); s++) {
				CVecD3 aux;
				stepsGroup.step[s]->electric.reserve(numNodes);
				stepsGroup.step[s]->magnetic.reserve(numNodes);
				for (uint i = 0; i < numNodes / 2; i++) {
					aux(0) = stepsGroup.step[s]->magnetic[i](0);
					aux(1) = stepsGroup.step[s]->magnetic[i](1);
					aux(2) = - stepsGroup.step[s]->magnetic[i](2);
					stepsGroup.step[s]->magnetic.push_back(aux);
					aux(0) = - stepsGroup.step[s]->electric[i](0);
					aux(1) = - stepsGroup.step[s]->electric[i](1);
					aux(2) = stepsGroup.step[s]->electric[i](2);
					stepsGroup.step[s]->electric.push_back(aux);
				}
			}
		}
	}
	if (sym[1] != Symmetry::none) {
		cout << "Not Done" << endl;
		exit(-1);
	}
	if (sym[2] != Symmetry::none) {
		nElem *= 2;
		numNodes *= 2;
		uint newVertexSize = 2 * vertex.size();
		vertex.reserve(newVertexSize);
		uint vertexCounter = newVertexSize / 2;
		for (uint i = 0; i < newVertexSize / 2; i++) {
			CVecD3 auxVec;
			auxVec(0) = vertex[i](0);
			auxVec(1) = - vertex[i](1);
			auxVec(2) = vertex[i](2);
			CoordD3 aux(vertexCounter++,auxVec);
			vertex.push_back(aux);
		}
		if (sym[2] == Symmetry::pec) {
			for (uint s = 0; s < stepsGroup.step.size(); s++) {
				CVecD3 aux;
				stepsGroup.step[s]->electric.reserve(numNodes);
				stepsGroup.step[s]->magnetic.reserve(numNodes);
				for (uint i = 0; i < numNodes / 2; i++) {
					aux(0) = - stepsGroup.step[s]->magnetic[i](0);
					aux(1) = stepsGroup.step[s]->magnetic[i](1);
					aux(2) = - stepsGroup.step[s]->magnetic[i](2);
					stepsGroup.step[s]->magnetic.push_back(aux);
					aux(0) = stepsGroup.step[s]->electric[i](0);
					aux(1) = - stepsGroup.step[s]->electric[i](1);
					aux(2) = stepsGroup.step[s]->electric[i](2);
					stepsGroup.step[s]->electric.push_back(aux);
				}
			}
		}
		if (sym[2] == Symmetry::pmc) {
			cout << "Not Done" << endl;
			exit(-1);
		}
	}
}
