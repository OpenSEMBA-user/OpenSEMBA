/*
 *        File: ParserNFDE.hpp
 * -------------------------------------------------------------------------
 *      Author: damarro
 *  Created on: Mar 3, 2014
 * Description:
 *              
 *              
 */

#ifndef PARSERNFDE_H_
#define PARSERNFDE_H_

#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

#include "NFDEData.h"

class ParserNFDE {
public:
	ParserNFDE(const string &input);
	NFDEData *parseNFDE();
	~ParserNFDE();
private:
	ifstream file;
	NFDEData *nfde;

	void parseTimeSteps();

	void parseSpaceSteps();
	void parseBackground();
	void parseBoundaries();

	void parsePlaneWaveSource();
	void parseCurrentDensitySource();
	void parseFieldSource();

	void parseIsotropicBody(const string &layer);
	void parseIsotropicSurf(const string &layer);
	void parseIsotropicLine(const string &layer);
	void parseAnisotropicBody(const string &layer);
	void parseAnisotropicSurf(const string &layer);
	void parseAnisotropicLine(const string &layer);
	void parseDispersiveBody(const string &layer);
	void parseDispersiveSurf(const string &layer);
	void parseDispersiveLine(const string &layer);
	void parseCompositeSurf(const string &layer);

	void parseThinWire(const string &layer);
	void parseThinGap(const string &layer);

	void parseTraditionalProbe(const string &layer);
	void parseNewProbe(const string &layer);
	void parseBulkProbe(const string &layer);
   void parseSliceProbe(const string &layer);
};

#endif /* PARSERNFDE_H_ */
