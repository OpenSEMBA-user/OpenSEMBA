/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef OUTPUTGIDMESH_H_
#define OUTPUTGIDMESH_H_

#include <stdlib.h>
#include <cstring>
#include <string>
#include "../gidpost/gidpost.h"
#include "../../common/NFDEData.h"
#include "../geometry/MeshVolume.h"

using namespace std;

#define GIDOUTPUT_ASCII_OUTPUT

class OutputGiDMesh {
public:
	OutputGiDMesh();
	OutputGiDMesh(
			const string& outputName_,
			const string& projectName_,
			const string& projectPath_);
	virtual ~OutputGiDMesh();
	void
	 write(const MeshVolume& mesh);
	void
	 write(const NFDE::NFDEData& mesh);
	void
	 flush() const;
private:
	string outputName;
	string projectName;
	string projectDir;
	GiD_PostMode GiDType;
	int coordCounter;
	int elemCounter;
	int coordCounterPreStart;// Counts elements already written.
	void
	 openGiDFiles();
	void
	 deleteExistentOutputFiles() const;
	void
	 initDefault();
	void
	 writeRectilinearMesh(const MeshVolume& mesh);
	void
	 writeQuad4Mesh(const string& name, const MeshVolume& mesh);
	void
	 writeHex8Mesh(const string& name, const MeshVolume& mesh);
};

#endif /* GIDOUTPUT_H_ */
