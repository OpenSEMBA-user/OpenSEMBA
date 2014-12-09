/*
 * ParserGiD.h
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#ifndef PARSERGID_H_
#define PARSERGID_H_
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "Parser.h"
#include "SmbData.h"
#include "ProblemSize.h"
#include "math/CartesianVector.h"

#define LABEL_ENDING ':'
#ifndef GIDPARSER_ERROR
#define GIDPARSER_ERROR 61594

class ParserGiD : public Parser {
public:
	ParserGiD();
	ParserGiD(
	 const string& fn);
	ParserGiD(
	 const string& fn,
	 const string& pTPath);
	virtual
	~ParserGiD();
	virtual SmbData*
	 read();
	virtual void
	 printInfo() const;
protected:
	string problemTypePath;
	ProblemSize pSize;
	virtual GlobalProblemData
	 readProblemData();
	virtual LayerGroup*
	 readLayers();
	virtual MeshVolume*
	 readMesh();
	virtual EMSourceGroup*
	 readEMSources();
	virtual OutputRequestGroup*
	 readOutputRequests();
	virtual OpenFOAMParameters*
	 readOpenFOAMParameters();
	virtual PhysicalModelGroup*
	 readMaterials();
	ProblemSize
	 readProblemSize();
	PMVolumeDispersive
	 readDispersiveMaterialFile(
	  const unsigned int id_,
	  const string& name) const;
	PMSurface
	 readIsotropicSurfaceMaterialFile(
	  const int id_,
	  const string& name) const;
	CoordinateGroup*
	 readCoordinates();
	ElementsGroup
	 readElements(const CoordinateGroup&);
	vector<Hex8>
	 readHex8Elements(const CoordinateGroup& v);
	vector<Tet10>
	 readTet10Elements(const CoordinateGroup& v);
	vector<Tet4>
	 readTet4Elements(const CoordinateGroup& v);
	vector<Tri6>
	 readTri6Elements(const CoordinateGroup& v);
	vector<Tri3>
	 readTri3Elements(const CoordinateGroup& v);
	vector<Lin2>
	 readLin2Elements(const CoordinateGroup& v);
	RectilinearGrid*
	 readCartesianGrid();
	vector<OutputRequest>
	 readOutputRequestInstances();
	void
	 getNextLabelAndValue(string& label, string& value);
private:
	typedef enum {
		outRqOnPoint,
		outRqOnLine,
		outRqOnSurface,
		outRqOnVolume,
		farField,
		undefined
	} GiDOutputType;
	pair<CVecD3, CVecD3>
	 readBoundFromStr(
	  const string& str) const;
	virtual PlaneWave
	 readPlaneWaveEMSource();
	virtual Dipole
	 readDipoleEMSource();
	virtual Waveport
	 readWaveportEMSource();
	virtual Generator
	 readGeneratorEMSource();
	Generator::Type
	 generatorStrToType(string label) const;
   Generator::Hardness
	 generatorStrToHardness(string label) const;
	GlobalProblemData::boundType
	 boundStrToType(string label) const;
	PhysicalModelGroup::Type
	 materialStrToType(string label) const;
	PMMultiport::Type
	 materialStrToMultiportType(string label) const;
	Element::Type
	 elementTypeStrToType(string label) const;
	OutputRequest::Type
	 outputTypeStrToType(string label) const;
	GiDOutputType
	 gidOutputTypeStrToType(string label) const;
	Domain
	 readDomainFromStr(const string& line) const;
	void
	 init(
	  const string& fn,
	  const string& pTPath);
	string
	 readVersion();
	bool
	 checkVersionCompatibility(const string version) const;
};

#endif /* PARSERGID_H_ */
