// File: electromagneticsource.h
#ifndef ELECTROMAGNETICSOURCE_H_
#define ELECTROMAGNETICSOURCE_H_

#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include "../Condition.h"
#include "Magnitude.h"
#include "MagnitudeGaussian.h"
#include "../../math/CartesianVector.h"
#include "../../math/Constants.h"

using namespace std;

class EMSource : public Condition {
public:
	EMSource();
	EMSource(
	        const vector<uint>& elem,
	        const Magnitude* magnitude);
    EMSource(
            const Magnitude* magnitude);
	virtual ~EMSource();
	bool
	 isEMSource() const;
	virtual bool
	 isPlaneWave() const;
	vector<unsigned int>
	 getElem() const;
	void
	 printInfo() const;
    const Magnitude* getMagnitude() const;

private:
	vector<unsigned int> elem_;
	const Magnitude* magnitude_;
};

#endif /* ELECTROMAGNETICSOURCE_H_ */
