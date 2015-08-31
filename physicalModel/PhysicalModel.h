#ifndef PHYSICALMODEL_H_
#define PHYSICALMODEL_H_

#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

#include "geometry/elements/GroupElements.h"
#include "math/Constants.h"
#include "math/CartesianVector.h"
#include "ProjectFile.h"

#include "base/class/ClassIdBase.h"

class PhysicalModel : public ClassIdBase<MatId> {
public:
    enum Type {
        PEC,
        PMC,
        SMA,
        classic,
        elecDispersive,
        anisotropic,
        isotropicsibc,
        PML,
        wire,
        multiport
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
