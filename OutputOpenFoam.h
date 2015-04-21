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
	  const ElementsGroup<const Tri>& tri,
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
