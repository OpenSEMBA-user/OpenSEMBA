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

#ifndef SEMBA_MESHER_OPENFOAM_PARSER_H_
#define SEMBA_MESHER_OPENFOAM_PARSER_H_

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

#include "Boundary.h"

#include "geometry/mesh/Unstructured.h"

#include "parser/Parser.h"

namespace SEMBA {
namespace Mesher {
namespace OpenFOAM {

typedef std::pair<Geometry::ElemId,
                  std::vector<Geometry::CoordId>> FaceIdentifier;

class Parser : public SEMBA::Parser::Parser {
public:
	Parser();
	Parser(const std::string& openfoamDir);
	virtual ~Parser();
	Geometry::Mesh::Unstructured readMeshUnstructured() const;
	bool isExistingDirectory(const std::string& dir) const;
	void printInfo() const;
private:
	std::string dirPolymesh_;
	Geometry::CoordR3Group readCoordinates() const;
	void skipHeader(std::ifstream& file) const;
	Geometry::ElemRGroup* readSurfaceElements(
            const Geometry::CoordR3Group&) const;
	std::vector<FaceIdentifier> readFaces() const;
	std::vector<std::size_t> readFacesOwner(
            const std::string& ownerOrNeighbour) const;
    std::vector<Boundary> readBoundaries() const;
	Geometry::Layer::Group<> assignAsLayers(
	        Geometry::ElemRGroup& elems,
	        const std::vector<Boundary>& boundaries) const;
	void openFile(std::ifstream& file, const std::string& name) const;
};

}
}
}

#endif /* SEMBA_MESHER_OPENFOAM_PARSER_H_ */
