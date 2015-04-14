/*
 * CommOutput.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#include "OutputComm.h"

OutputComm::OutputComm(
      const SmbData* smb,
      const DG* dg,
      const Comm* comm) :
      Output(smb, dg) {
   comm_ = comm;
   lastSyncing_ = (double) 0.0;
}

OutputComm::~OutputComm() {

}

void
OutputComm::writeResults(
      const FieldD3& electric,
      const FieldD3& magnetic,
      CVecD3& EInc,
      const double time, const uint tStep) {
   if ((time != lastSyncing_) || (tStep == 0)) {
      comm_->gatherFieldsSlave(electric, magnetic);
      lastSyncing_ = time;
   }
}
