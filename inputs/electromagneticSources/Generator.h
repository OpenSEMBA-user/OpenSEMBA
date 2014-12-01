/*
 * Generator.h
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_

#include "EMSource.h"

class Generator : public EMSource {
public:
	typedef enum {
		voltage = 1,
		current = 2,
		undefined = 0
	} Type;
	Generator();
	Generator(
	 const Type& generatorType,
	 const vector<unsigned int>& elem,
	 const double spread,
	 const double delay,
	 const string& filename_);
	virtual ~Generator();
	Generator&
	 operator=(const Generator &rhs);
	Type
	 getType() const;
	virtual void
	 printInfo() const;
private:
	Type type;
	string
	 getTypeStr() const;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_ */
