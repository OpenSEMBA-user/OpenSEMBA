/*
 * Nodal.h
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_

#include "EMSource.h"

class Nodal : public EMSource {
public:
	typedef enum {
		electricField = 1,
		magneticField = 2,
		undefined = 0
	} Type;
   typedef enum {
		hard = 1,
		soft = 2
	} Hardness;
	Nodal();
	Nodal(
	 const Type& sourceType,
     const Hardness& sourceHardness,
	 const vector<unsigned int>& elem,
	 const Magnitude* magnitude);
	virtual ~Nodal();
	Nodal&
	 operator=(const Nodal &rhs);
	Type
	 getType() const;
   Hardness
	 getHardness() const;
	virtual void
	 printInfo() const;
private:
	Type type_;
   Hardness hardness_;
	string
	 getTypeStr() const;
   string
	 getHardnessStr() const;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_NODALSOURCE_H_ */
