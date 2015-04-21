/*
 * Graph.h
 *
 *  Created on: Aug 23, 2012
 *      Author: luis
 */

#ifndef RESULTGRAPH_H_
#define RESULTGRAPH_H_

#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

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
         const Real time,
         const FieldR3& electric,
         const FieldR3& magnetic);
private:
   ofstream file_;
};

#endif /* GRAPH_H_ */
