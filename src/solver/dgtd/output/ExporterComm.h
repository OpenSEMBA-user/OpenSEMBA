/*
 * NoOutput.h
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#ifndef OUTPUTCOMM_H_
#define OUTPUTCOMM_H_

#include "exporter/Exporter.h"
#include "Comm.h"

using namespace std;

class ExporterComm : public Exporter {
public:
   ExporterComm(
         const SmbData* smb,
         const Comm* comm);
   virtual	~ExporterComm();
   void
   closeResultsFile();
   void
   writeResults(
         const FieldR3& electric,
         const FieldR3& magnetic,
         CVecR3& EInc,
         const double time, const uint tStep);
private:
   double lastSyncing_;
   const Comm* comm_;
};

#endif
