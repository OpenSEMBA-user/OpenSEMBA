/*
 * VectorModuleResult.cpp
 *
 *  Created on: Aug 22, 2012
 *      Author: luis
 */
#ifndef VECTORMODULERESULT_H_
#include "VectorModuleResult.h"
#endif

VectorModuleResult::VectorModuleResult(const UInt numberOfValues_) {
	where = 0.0;
	numberOfValues = numberOfValues_;
	valuePos.resize(numberOfValues, 0);
	for (UInt i = 0; i < 3; i++)
		values[i].resize(numberOfValues, 0.0);
}

VectorModuleResult::~VectorModuleResult() {
	// TODO Auto-generated destructor stub
}

VectorModuleResult&
VectorModuleResult::operator=(VectorModuleResult& rhs) {
	if (this == &rhs)
		return *this;
	numberOfValues = rhs.numberOfValues;
	xLabel = rhs.xLabel;
	yLabel = rhs.yLabel;
	where  = rhs.where;
	for (UInt i = 0; i < 4; i++)
		componentNames[i] = rhs.componentNames[i];
	valuePos = rhs.valuePos;
	for (UInt i = 0; i < 3; i++)
		values[i] = rhs.values[i];
	return *this;
}

void
VectorModuleResult::readResult(ifstream& f_in) {
	// --- Variables --------------------------------------------------
	string line, label;
	string resultLabel;
	string componentsNameLabel;
	string vectorLabel, vectorPosition;
	string valueLabel, endValueLabel;
	size_t pos1, pos2;
	// --- Body -------------------------------------------------------
	// Main loop running over the file lines.
	bool resultFound = false;
	bool finished = false;
	while (f_in.eof() || !finished) {
		// Checks if current line contains the xLabels.
		getline(f_in, line);
		pos1 = line.find(' ');
		resultLabel = line.substr(0, pos1);
		line = line.substr(pos1, line.length() - pos1);
		if (resultLabel == "Result") {
			resultFound = true;
			// Reads xLabel.
			pos1 = line.find('"');
			pos2 =
			 line.substr(pos1 + 1, line.length() - pos1).find('"');
			xLabel = '"' + line.substr(pos1 + 1, pos2 - pos1 + 1) + '"';
			line = line.substr(pos2 + 3, line.length());
			// Reads yLabel.
			pos1 = line.find('"');
			pos2 =
			 line.substr(pos1 + 1, line.length() - pos1).find('"');
			yLabel = '"' + line.substr(pos1 + 1, pos2 - pos1 + 1) + '"';
			line = line.substr(pos2 + 3, line.length());
			// Reads where.
			pos1 = line.find(' ');
			pos2 =
			 line.substr(pos1 + 1, line.length() - pos1).find(' ');
			where = atof(line.substr(pos1 + 1, pos2 - pos1 + 1).c_str());
			line = line.substr(pos2 + 1, line.length());
			// Reads vectorLabel.
			pos1 = line.find(' ');
			pos2 =
			 line.substr(pos1 + 1, line.length() - pos1).find(' ');
			vectorLabel =
			 line.substr(pos1 + 1, pos2 - pos1 + 1).c_str();
			line = line.substr(pos2 + 1, line.length());
			// Reads vectorPosition.
			pos1 = line.find(' ');
			vectorPosition =
			 line.substr(pos1 + 1, line.length() - pos1).c_str();
		}
		// Reads component names.
		f_in >> componentsNameLabel
			 >> componentNames[0] >> componentNames[1]
			 >> componentNames[2] >> componentNames[3];
		// Reads values contained in the result.
		f_in >> valueLabel;
		for (UInt i = 0; i < numberOfValues; i++) {
			f_in >> valuePos[i]
				 >> values[0][i] >> values[1][i] >> values[2][i];
		}
		// Reads ending Label.
		getline(f_in, line);
		getline(f_in, endValueLabel);
		if (endValueLabel == "End Values")
			finished = true;
		// Errors handling.
		if (f_in.eof() && !resultFound) {
			cerr << endl << "ERROR @ Output:: readResult" << endl;
			cerr << endl << "End Of File reached  without finding result."
				 << endl;
			cerr << endl << "Terminating." << endl;
			exit(VECTORRESULT_ERROR);
		}
		if (f_in.eof() && !finished) {
			cerr << endl << "ERROR @ Output:: readResult" << endl;
			cerr << endl << "End Of File reached  without result end." << endl;
			cerr << endl << "Terminating." << endl;
			exit(VECTORRESULT_ERROR);
		}
	} // End of while (not finished).
}

void
VectorModuleResult::printInfo() const {
	cout << "----- Vector result information -----" << endl;
	cout << "X label: " << xLabel << endl;
	cout << "Y label: " << yLabel << endl;
	cout << "Component names: ";
	for (UInt i = 0; i < 4; i++)
		cout << componentNames[i] << " ";
	cout << endl;
	cout << "This result contains "
		 << valuePos.size() << " results" << endl;
	cout << "Value Pos - "
		 << componentNames[0] << " - "
		 << componentNames[1] << " - "
		 << componentNames[2] << endl;
	for (UInt i = 0; i < valuePos.size() && i < 5; i++) {
		cout << valuePos[i] << " "
			 << values[0][i] << " "
			 << values[1][i] << " "
			 << values[2][i] << endl;
	}
}
