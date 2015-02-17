#ifndef CONDITION_H_
#define CONDITION_H_

#include <stdlib.h>
#include <assert.h>

#include "../geometry/elements/Element.h"

using namespace std;

class Condition {
public:
   typedef enum {
      pec,
      pmc,
      sma,
      sibc,
      emSource,
      undefined
   } Type;
	Condition();
	Condition(Element::Type param);
	virtual ~Condition();
	virtual bool
	 isSMA() const;
	virtual bool
	 isPEC() const;
	virtual bool
	 isPMC() const;
	virtual bool
	 isVacuum() const {return false;}
	virtual bool
	 isPMPredefined() const {return false;}
	virtual bool
	 isSurface() const {return false;}
	virtual bool
	 isSurfaceImpedance() const {return false;}
	virtual bool
	 isPhysicalModel() const {return false;}
	virtual bool
	 isEMSource() const {return false;}
	virtual bool
	 isElectricalDispersive() const {return false;}
	virtual unsigned int
	 getId() const;
	Element::Type
	 getElementType() const {
		return elementType;
	 }
	virtual Condition::Type
	 getConditionType() const;
	virtual void
	 printInfo() const = 0;
private:
	Element::Type elementType;
};
#endif /* CONDITION_H_ */
