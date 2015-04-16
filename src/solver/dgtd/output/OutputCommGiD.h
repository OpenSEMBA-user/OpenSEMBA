/*
 * CommGiDOutput.h
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#ifndef COMMGIDOUTPUT_H_
#define COMMGIDOUTPUT_H_

#include "OutputGiD.h"
#include "../core/Comm.h"

class OutputCommGiD : public OutputGiD {
public:
   OutputCommGiD(
         const SmbData* smb,
         const DG* dg,
         const Comm* comm);
   virtual	~OutputCommGiD();
   virtual void process(
         const double time,
         const FieldD3& elec,
         const FieldD3& magn);
private:
   const Comm* comm_;
   double lastSyncing_;
};

#endif /* COMMGIDOUTPUT_H_ */
