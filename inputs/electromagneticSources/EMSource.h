// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include "Condition.h"
#include "../math/CartesianVector.h"
#include "../math/Constants.h"

using namespace std;

class EMSource : public Condition {
public:
	EMSource();
	virtual ~EMSource();
	bool
	 isEMSource() const;
	virtual bool
	 isPlaneWave() const;
	vector<unsigned int>
	 getElem() const;
	double
	 getDelay() const;
	const string&
	 getFilename() const;
	double
	 getSpread() const;
protected:
	vector<unsigned int> elem;
	double spread; // Time spread of gauss. pulse.
	double delay; // Peak time of gauss. pulse.
	string filename;
	virtual void
	 printMagnitude() const;
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
