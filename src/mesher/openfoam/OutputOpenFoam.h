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
 * OutOpenFoam.h
 *
 *  Created on: Apr 1, 2014
 *      Author: luis
 */

#ifndef OUTOPENFOAM_H_
#define OUTOPENFOAM_H_

#include "SmbData.h"

class OutputOpenFoam : public ProjectFile {
public:
    OutputOpenFoam();
    OutputOpenFoam(const SmbData* smb);
    virtual ~OutputOpenFoam();
private:
	const SmbData* smb_;
	string dirConstant_, dirPolymesh_, dirTriSurface_;
	string dirSystem_;
	typedef enum {
		dictionary,
	} classes;
	void writeAllClean() const;
	void writeAllRun() const;
//	void writeAllRunParallel() const;
	void writeSTLs() const;
	void triToSTL(
	  const GroupElements<const Triangle>& tri,
	  const string& folder,
	  const string& type,
	  const UInt& typeId,
	  const string& name) const;
	void writeControlDict() const;
	void writefvSchemes() const;
	void writefvSolution() const;
	void writeBlockMeshDict() const;
	void writeMeshQualityDict() const;
//	void writeDecomposeParDict() const;
	void createOpenFoamDirs();
	string writeOpenFoamHeader(
	  const string& location,
	  const string& object) const;
	string writeAllBoundary() const;
	void writeOpenFoamDummyFile() const;
	void writeSurfaceFeatureExtractDict() const;
	void writeSnappyHexMeshDict() const;
	UInt computeRefinableCellDim(
	  const UInt originalCellDim,
	  const UInt numberOfProcessors) const;
	string boolToStr(const bool) const;
	string intToStr(const UInt i) const;
	CVecR3 computeLocationInMesh() const;
};

#endif /* OUTOPENFOAM_H_ */
