#ifndef OUTPUTNFDE_H_
#define OUTPUTNFDE_H_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <ctime>
#include <cmath>

using namespace std;

#include "NFDEData.h"
#include "ProjectFile.h"

class OutputNFDE : public ProjectFile, public NFDEData {
public:
	OutputNFDE(const NFDEData *nfde);
	~OutputNFDE();
	void exportNFDE(const string &filename,
                   bool outputHeaders = true,
                   bool outputEnd = true);
private:
	const NFDEData *nfde;
	ofstream output;
    static const string space;

    void exportTimeSteps();

	void exportSpaceSteps();
	void exportBackground();
	void exportBoundaries();

	void exportPlaneWaveSource();
	void exportCurrentDensitySource();
	void exportFieldSource();

	void exportIsotropicBody();
	void exportIsotropicSurf();
	void exportIsotropicLine();
	void exportAnisotropicBody();
	void exportAnisotropicSurf();
	void exportAnisotropicLine();
	void exportDispersiveBody();
	void exportDispersiveSurf();
	void exportDispersiveLine();
	void exportCompositeSurf();

	void exportThinWire();
	void exportThinGap();

	void exportTraditionalProbe();
	void exportNewProbe();
	void exportBulkProbes();
	void exportSliceProbes();

	string
	probeName(const string& name, const string& layer, const bool log = false) const;
};

#endif /* OUTPUTNFDE_H_ */
