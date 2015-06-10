/*
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#include "OutputCommGiD.h"

OutputCommGiD::OutputCommGiD(
      const SmbData* smb,
      const DG* dg,
      const Comm* comm) : OutputGiD(smb, dg) {
   comm_ = comm;
   lastSyncing_ = (double) 0.0;
}

OutputCommGiD::~OutputCommGiD() {
   // TODO Auto-generated destructor stub
}

void
OutputCommGiD::process(
      const double time,
      const FieldD3& localElec,
      const FieldD3& localMagn) {
   uint fSize = comm_->getGlobalSize() * dg_->np;
   FieldD3 electric(fSize), magnetic(fSize);
   if ((time != lastSyncing_) || (time == 0.0)) {
      comm_->gatherFieldsMaster(electric, magnetic, localElec, localMagn);
      lastSyncing_ = time;
   }
   Exporter::process(time, electric, magnetic);
}
