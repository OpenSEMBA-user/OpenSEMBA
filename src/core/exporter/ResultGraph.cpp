// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

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
