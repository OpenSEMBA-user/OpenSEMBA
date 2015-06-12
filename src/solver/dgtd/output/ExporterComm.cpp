/*
 * CommOutput.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#include "ExporterComm.h"

ExporterComm::ExporterComm(
      const SmbData* smb,
      const DG* dg,
      const Comm* comm) :
      Output(smb, dg) {
   comm_ = comm;
   lastSyncing_ = (double) 0.0;
}

ExporterComm::~ExporterComm() {

}

void
ExporterComm::writeResults(
      const FieldR3& electric,
      const FieldR3& magnetic,
      CVecR3& EInc,
      const double time, const uint tStep) {
   if ((time != lastSyncing_) || (tStep == 0)) {
      comm_->gatherFieldsSlave(electric, magnetic);
      lastSyncing_ = time;
   }
}
