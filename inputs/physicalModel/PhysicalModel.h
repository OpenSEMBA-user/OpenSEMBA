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

using namespace std;

#ifndef PHYSICALMODEL_ERROR
	#define PHYSICALMODEL_ERROR 74721
#endif

class PhysicalModel : public Condition {
	friend class Parser;
public:
	PhysicalModel();
	PhysicalModel(const uint id, const string& name);
	virtual ~PhysicalModel();
	virtual unsigned int
	 getId() const {return id_;}
	const string&
	 getName() const;
	virtual double
	 getImpedance() const;
	virtual double
	 getAdmitance() const;
	bool
	 isPhysicalModel() const {return true;}
	virtual bool
	 isSurfaceImpedance() const {return false;}
   virtual bool
    isSurfaceMultilayer() const {return false;}
	virtual bool
	 isSurfaceType() const {return false;}
	virtual bool
	 isSMA() const {return false;}
	virtual bool
	 isPEC() const {return false;}
	virtual bool
	 isPMC() const {return false;}
	virtual bool
	 isVolumic() const {return false;}
	virtual bool
	 isDispersive() const {return false;}
	virtual bool
	 isWire() const {return false;}
	virtual bool
	 isMultiport() const {return false;}
	virtual bool
	 isMultiportRLC() const {return false;}
	virtual bool
	 isClassic() const {return false;}
	virtual bool
	 isPML() const;
	virtual void
     printInfo() const = 0;
protected:
	static const double eps0;
	static const double mu0;
	unsigned int id_;
	string name_;
private:
};

#endif
