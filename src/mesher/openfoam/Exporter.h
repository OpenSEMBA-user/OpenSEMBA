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

#ifndef SEMBA_MESHER_OPENFOAM_EXPORTER_H_
#define SEMBA_MESHER_OPENFOAM_EXPORTER_H_

#include "Data.h"
#include "geometry/element/Triangle.h"

namespace SEMBA {
namespace Mesher {
namespace OpenFOAM {

class Exporter : public FileSystem::Project {
public:
    Exporter();
    Exporter(const Data* smb);
    virtual ~Exporter();
private:
	const Data* smb_;
	std::string dirConstant_, dirPolymesh_, dirTriSurface_;
    std::string dirSystem_;
	typedef enum {
		dictionary,
	} classes;
	void writeAllClean() const;
	void writeAllRun() const;
//	void writeAllRunParallel() const;
	void writeSTLs() const;
	void triToSTL(const Geometry::Element::Group<const Geometry::Tri>& tri,
	              const std::string& folder,
	              const std::string& type,
	              const std::size_t& typeId,
	              const std::string& name) const;
	void writeControlDict() const;
	void writefvSchemes() const;
	void writefvSolution() const;
	void writeBlockMeshDict() const;
	void writeMeshQualityDict() const;
//	void writeDecomposeParDict() const;
	void createOpenFoamDirs();
    std::string writeOpenFoamHeader(const std::string& location,
                                    const std::string& object) const;
	std::string writeAllBoundary() const;
	void writeOpenFoamDummyFile() const;
	void writeSurfaceFeatureExtractDict() const;
	void writeSnappyHexMeshDict() const;
	std::size_t computeRefinableCellDim(
            const std::size_t originalCellDim,
            const std::size_t numberOfProcessors) const;
	std::string boolToStr(const bool) const;
    std::string intToStr(const std::size_t i) const;
	Math::CVecR3 computeLocationInMesh() const;
};

}
}
}

#endif /* SEMBA_MESHER_OPENFOAM_EXPORTER_H_ */
