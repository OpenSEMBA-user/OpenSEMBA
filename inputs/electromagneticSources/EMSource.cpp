#ifndef ELECTROMAGNETICSOURCE_H_
#	include "EMSource.h"
#endif
// =============== ElectromagneticSource methods ==============================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// =-=-=-=-=-=-=-= Destructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
EMSource::EMSource() {
	spread = 0.0;
	delay = 0.0;
}

EMSource::~EMSource() {

}

vector<unsigned int>
EMSource::getElem() const {
	if (elem.size() != 0) {
		return elem;
	} else {
		cerr << "ERROR @ EMSource::getElem" << endl;
		cerr << "Element Ids vector was not initialized." << endl;
		assert(false);
		exit(-1);
	}
}

void
EMSource::printMagnitude() const {
	cout << " - Gaussian spread: " << spread << endl;
	cout << " - Gaussian delay:	" << delay << endl;
	cout << " - Excitation filename: " << filename << endl;
}

bool
EMSource::isEMSource() const {
	return true;
}

bool
EMSource::isPlaneWave() const {
	return false;
}

double
EMSource::getDelay() const {
	return delay;
}

const string&
EMSource::getFilename() const {
	return filename;
}

double
EMSource::getSpread() const {
	return spread;
}
