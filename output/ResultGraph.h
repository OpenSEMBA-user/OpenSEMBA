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

#include "ProjectFile.h"
#include "Result.h"

using namespace std;

class ResultGraph : public ProjectFile, public Result {
public:
	ResultGraph();
	ResultGraph(
	 const string& name,
	 const string& xLabel,
	 const string& yLabel);
	virtual ~ResultGraph();
   virtual void write(
         const double time,
         const FieldD3& electric,
         const FieldD3& magnetic);
private:
   ofstream file_;
};

#endif /* GRAPH_H_ */
