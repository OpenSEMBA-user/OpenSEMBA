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
	bool isPhysicalModel() const {return true;}

	virtual bool isSurfaceImpedance() const {return false;}
	virtual bool isSurfaceMultilayer() const {return false;}
	virtual bool isSurfaceType() const {return false;}

	virtual bool isSMA() const {return false;}
	virtual bool isPEC() const {return false;}
	virtual bool isPMC() const {return false;}
	virtual bool isPML() const {return false;};

	virtual bool isVolumic() const {return false;}
	virtual bool isClassic() const {return false;}
	virtual bool isDispersive() const {return false;}

	virtual bool isWire() const {return false;}
	virtual bool isMultiport() const {return false;}
	virtual bool isMultiportRLC() const {return false;}

	virtual void printInfo() const;

	virtual ClassBase* clone() const;
    virtual ClassBase* clone(const uint id) const;
private:
    string name_;
};


#endif
