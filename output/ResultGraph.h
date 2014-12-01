/*
 * Graph.h
 *
 *  Created on: Aug 23, 2012
 *      Author: luis
 */

#ifndef RESULTGRAPH_H_
#define RESULTGRAPH_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <complex>
#include <cmath>
#include <vector>
#include <string>
#include <utility>

using namespace std;

#define GRAPH_EXPECTED_SIZE 200000

class ResultGraph {
public:
	ResultGraph();
	ResultGraph(
	 const string& name,
	 const string& xLabel,
	 const string& yLabel);
	ResultGraph(
	 const string& projectFolder,
	 const string& name,
	 const string& xLabel,
	 const string& yLabel);
	virtual ~ResultGraph();
	void
	 append(double x, double y);
	void
	 writeInFile() const;
private:
	string fileName_, name_;
	string xLabel_, yLabel_;
	vector<double> x_, y_;
};

#endif /* GRAPH_H_ */
