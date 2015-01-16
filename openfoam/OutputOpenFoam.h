/*
 * OutOpenFoam.h
 *
 *  Created on: Apr 1, 2014
 *      Author: luis
 */

#ifndef OUTOPENFOAM_H_
#define OUTOPENFOAM_H_

#include "../../common/SmbData.h"

class OutputOpenFoam : public ProjectFile {
public:
	OutputOpenFoam();
	OutputOpenFoam(
	 const SmbData* smb,
	 const bool bruteForceVolumes
 	);
	virtual ~OutputOpenFoam();
private:
	const SmbData* smb_;
	string dirConstant_, dirPolymesh_, dirTriSurface_;
	string dirSystem_;
	bool bruteForceVolumes_;
	typedef enum {
		dictionary,
	} classes;
	void
	 writeAllClean() const;
	void
	 writeAllRun() const;
	void
	 writeAllRunParallel() const;
	void
	 writeSTLs() const;
	void
	 triToSTL(
	  const vector<Tri3>& tri,
	  const string& folder,
	  const string& type,
	  const uint& typeId,
	  const string& name) const;
	void
	 writeControlDict() const;
	void
	 writefvSchemes() const;
	void
	 writefvSolution() const;
	void
	 writeBlockMeshDict() const;
	void
	 writeMeshQualityDict() const;
	void
	 writeDecomposeParDict() const;
	void
	 createOpenFoamDirs();
	string
	 writeOpenFoamHeader(
	  const string& location,
	  const string& object) const;
	string
	 writeAllBoundary() const;
	void
	 writeOpenFoamDummyFile() const;
	void
	 writeSurfaceFeatureExtractDict() const;
	void
	 writeSnappyHexMeshDict() const;
	uint
	 computeRefinableCellDim(
	  const uint originalCellDim,
	  const uint numberOfProcessors) const;
	string
	 boolToStr(const bool) const;
	string
	 intToStr(const uint i) const;
	CVecD3
	 computeLocationInMesh() const;
};

#endif /* OUTOPENFOAM_H_ */
