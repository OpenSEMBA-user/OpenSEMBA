/*
 * ArgumentsSmbToNFDE.h
 *
 *  Created on: Apr 11, 2014
 *      Author: mdebi
 */

#ifndef ARGUMENTSSMBTONFDE_H_
#define ARGUMENTSMESHER_H_

#include "../../common/inputs/Arguments.h"
#include "../../common/inputs/MesherOptions.h"

class ArgumentsMesher: public Arguments {
public:
   ArgumentsMesher(const int argc, const char* argv[]);
   virtual ~ArgumentsMesher();
   virtual void set(
           MesherOptions* params,
           const Int argc_,
           const char* argv[]) const;
   void printInfo() const;
private:
   void printHelp() const;
};

#endif /* ARGUMENTSSMBTONFDE_H_ */
