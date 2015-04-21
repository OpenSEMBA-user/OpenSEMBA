#ifndef PHYSICALMODEL_H_
#define PHYSICALMODEL_H_

#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

#include "geometry/elements/ElementsGroup.h"
#include "math/Constants.h"
#include "math/CartesianVector.h"

#include "base/class/ClassIdBase.h"

class PhysicalModel : public ClassIdBase<MatId> {
public:
    enum Type {
        vacuum = 1,
        PEC = 2,
        PMC = 3,
        SMA = 4,
        classic = 5,
        elecDispersive = 6,
        isotropicsibc = 7,
        PML = 8,
        wire = 9,
        multiport = 10,
        undefined = 0
    };

	PhysicalModel(const MatId id, const string& name);
	virtual ~PhysicalModel();

	const string& getName() const;
	void setName(const string& newName);

	virtual void printInfo() const;
private:
    string name_;
};


#endif
