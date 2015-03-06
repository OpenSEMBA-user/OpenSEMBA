#ifndef PHYSICALMODEL_H_
#define PHYSICALMODEL_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <complex>
#include <cmath>
#include <vector>
#include "../Condition.h"
#include "../../math/Constants.h"
#include "../../math/CartesianVector.h"
#include "ClassWithIdBase.h"

using namespace std;

class PhysicalModel : public ClassWithIdBase<uint>, public Condition {
public:
	PhysicalModel(const uint id, const string& name);
	virtual ~PhysicalModel();

	const string& getName() const;
	void setName(const string& newName);

	virtual void printInfo() const;
private:
    string name_;
};


#endif
