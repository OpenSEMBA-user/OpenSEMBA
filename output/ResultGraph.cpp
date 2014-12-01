/*
 * Graph.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef RESULTGRAPH_H_
	#include "ResultGraph.h"
#endif

ResultGraph::ResultGraph() {

}

ResultGraph::ResultGraph(
 const string& name,
 const string& xLabel,
 const string& yLabel) {
	fileName_ = name + ".grf";
	name_ = name;
	xLabel_ = xLabel;
	yLabel_ = yLabel;
	x_.reserve(GRAPH_EXPECTED_SIZE);
	y_.reserve(GRAPH_EXPECTED_SIZE);
}

ResultGraph::ResultGraph(
 const string& projectFolder,
 const string& name,
 const string& xLabel,
 const string& yLabel) {
	fileName_ = projectFolder + name + ".grf";
	name_ = name;
	xLabel_ = xLabel;
	yLabel_ = yLabel;
	x_.reserve(GRAPH_EXPECTED_SIZE);
	y_.reserve(GRAPH_EXPECTED_SIZE);
}

ResultGraph::~ResultGraph() {
	writeInFile();
}

void
ResultGraph::append(double x, double y) {
	x_.push_back(x);
	y_.push_back(y);
}

void
ResultGraph::writeInFile() const {
	ofstream file(fileName_.c_str());
	// Writes data.
	file << "#Graph: \"" << name_ << "\"" << endl;
	file << "#X: \"" << xLabel_ << "\" Y: \"" << yLabel_ << "\"" << endl;
	// Writes bulk of data.
	for (uint i = 0; i < x_.size(); i++) {
		file << x_[i] << " " << y_[i] << endl;
	}
	// Writes endings.
	file << "# End" << endl;
	//
	file.close();
}
//void Output::writeScalarGraph(const Probe& p) const {
//	// Builds probe names and probe file names.
//	string probeName = "probe " + p.name;
//	// Opens file, write only mode.
//	ofstream f_out;
//	string probeFileName = projectFolder + projectName
//			+ ".probe." + p.name  + ".grf";
//	cout << "Writing graph in: " << probeFileName << endl;
//	f_out.open(probeFileName.c_str(), ifstream::out);
//	// Writes graph label.
//	string yLabel;
//	if (p.isPower()) {
//		yLabel = "W";
//	} else {
//		cerr << "ERROR @ Output::writeScalarGraph()" << endl;
//		cerr << "Unreckognized output type." << endl;
//		cerr << "Terminating." << endl;
//		exit(OUTPUT_ERROR);
//	}
//	// Writes stored results in file.
//	string xLabel = "Time";
//	string pName = probeName + " power";
////	writeGraph(f_out, pName, xLabel, p.time, yLabel, p.val); TODO
//	// Closes file.
//	f_out.close();
//}
// 
//void Output::writeVectorGraph(const Probe& p) const {
//	// Builds probe names and probe file names.
//	string probeName = "probe " + p.name;
//	// Opens file, write only mode.
//	ofstream f_out;
//	string probeFileName = projectFolder + projectName
//			               + ".probe." + p.name  + ".grf";
//	cout << "Writing graph in: " << probeFileName << endl;
//	f_out.open(probeFileName.c_str(), ifstream::out);
//	// Writes graph label.
//	string lx, ly, lz, lAbs;
//	if (p.isElectricField()) {
//		lx = "Ex";
//		ly = "Ey";
//		lz = "Ez";
//		lAbs = "|E|";
//	} else if (p.isMagneticField()) {
//		lx = "Hx";
//		ly = "Hy";
//		lz = "Hz";
//		lAbs = "|H|";
//	} else if (p.isPowerDensity()) {
//		lx = "Sx";
//		ly = "Sy";
//		lz = "Sz";
//		lAbs = "|S|";
//	} else {
//		cerr << "ERROR @ Output::writeVectorGraph()" << endl;
//		cerr << "Unreckognized output type." << endl;
//		cerr << "Terminating." << endl;
//		exit(OUTPUT_ERROR);
//	}
//	// Writes stored results in file.
//	vector<double> fx, fy, fz, fAbs;
//	fx = getCartesianVectorComponent(p.vecVal, 0);
//	fy = getCartesianVectorComponent(p.vecVal, 1);
//	fz = getCartesianVectorComponent(p.vecVal, 2);
//	fAbs = p.val;
////	writeGraph(f_out, probeName + " " + lx, "Time", p.time, lx, fx); TODO
////	writeGraph(f_out, probeName + " " + ly, "Time", p.time, ly, fy);
////	writeGraph(f_out, probeName + " " + lz, "Time", p.time, lz, fz);
////	writeGraph(f_out, probeName + " " + lAbs, "Time", p.time, lAbs, fAbs);
//	// Closes file.
//	f_out.close();
//}
// 
//void Output::writeGraph(ofstream& f_out, const string& probeName,
//		      const string& xLabel, const vector<double>& x,
//		      const string& yLabel, const vector<double>& y) const {
//	// Writes graphs headers.
//	f_out << "#Graph: \"" << probeName << "\"" << endl;
//	f_out << "#X: \"" << xLabel << "\" Y: \"" << yLabel << "\"" << endl;
//	// Writes bulk of data.
//	for (uint i = 0; i < x.size(); i++)
//		f_out << x[i] << " " << y[i] << endl;
//	// Writes endings.
//	f_out << "# End" << endl;
//}
// 
//vector<double> Output::getCartesianVectorComponent(
//		                const vector<CartesianVector<double,3> >& vec,
//		                int pos) const {
//	vector<double> res(vec.size(), 0.0);
//	for (uint i = 0; i < vec.size(); i++)
//		res[i] = vec[i].val[pos];
//	return res;
//}
// 
//vector<double> Output::getCartesianVectorNorm(
//		                const vector<CartesianVector<double,3> >& vec) const {
//	vector<double> res(vec.size(), 0.0);
//	for (uint i = 0; i < vec.size(); i++)
//		res[i] = vec[i].norm();
//	return res;
//}

