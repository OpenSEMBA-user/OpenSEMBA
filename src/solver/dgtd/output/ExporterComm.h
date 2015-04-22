/*
 * NoOutput.h
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#ifndef OUTPUTCOMM_H_
#define OUTPUTCOMM_H_

#include "../../../common/exporter/Output.h"
#include "../core/Comm.h"

using namespace std;

class OutputComm : public Exporter {
public:
   OutputComm(
         const SmbData* smb,
         const DG* dg,
         const Comm* comm);
   virtual	~OutputComm();
   void
   closeResultsFile();
   void
   writeResults(
         const FieldD3& electric,
         const FieldD3& magnetic,
         CVecD3& EInc,
         const double time, const uint tStep);
private:
   double lastSyncing_;
   const Comm* comm_;
};

#endif
