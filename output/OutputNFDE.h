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

class OutputNFDE : public ProjectFile {
    friend class OutputGiD;
public:
	OutputNFDE(const NFDEData *nfde);
	~OutputNFDE();
	void exportNFDE(const string &filename,
                   bool outputHeaders = true,
                   bool outputEnd = true);
protected:

    string toString(const NFDEData::MaterialTypes::value mat);

    static string toString(CartesianAxis dir);
    static string toString(CartesianBound pos);
    template<class T, int D>
    static string toString(const CartesianVector<T, D>& vec);
    static string toString1PNT(const NFDEData::Coords coord);
    static string toString2PNT(const NFDEData::Coords coord);
    static string toString2PNT(const NFDEData::CoordsDir coord, int skip = 0);

    static string toString(const NFDEData::CoordsMultiplier coord);
    static string toString(
            const NFDEData::Boundary& boundary,
            const uint d = 3,
            const uint p = 2);
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

template<class T, int D>
string OutputNFDE::toString(const CartesianVector<T, D>& vec) {
    stringstream res;
    for (int d = 0; d < D; d++)
        res << vec(d) << " ";
    return res.str();
}

#endif /* OUTPUTNFDE_H_ */
