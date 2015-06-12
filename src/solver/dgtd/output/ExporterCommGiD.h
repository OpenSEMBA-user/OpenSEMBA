/*
 * CommGiDOutput.h
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#ifndef COMMGIDOUTPUT_H_
#define COMMGIDOUTPUT_H_

#include "ExporterGiD.h"
#include "../core/Comm.h"

class ExporterCommGiD : public ExporterGiD {
public:
   ExporterCommGiD(
         const SmbData* smb,
         const Comm* comm);
   virtual	~ExporterCommGiD();
   virtual void process(
         const double time,
         const FieldR3& elec,
         const FieldR3& magn);
private:
   const Comm* comm_;
   double lastSyncing_;
};

#endif /* COMMGIDOUTPUT_H_ */
