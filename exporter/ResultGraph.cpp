
/*
 * Graph.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef RESULTGRAPH_H_
	#include "../exporter/ResultGraph.h"
#endif

ResultGraph::ResultGraph() {

}

ResultGraph::ResultGraph(
 const string& filename,
 const string& xLabel,
 const string& yLabel) : ProjectFile(filename) {
	openFile(file_);
   file_ << "#Graph: \"" << getBasename() << "\"" << endl;
   file_ << "#X: \"" << xLabel << "\" Y: \"" << yLabel << "\"" << endl;
}

ResultGraph::~ResultGraph() {
   file_ << "# End" << endl;
   file_.close();
}

void
ResultGraph::write(
      const Real time,
      const FieldR3& electric,
      const FieldR3& magnetic) {
   assert(electric.size() == 1);
   assert(magnetic.size() == 0);
	file_ << time << " " << electric.getCVec(0).norm() << endl;
}
