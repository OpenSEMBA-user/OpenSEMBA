/*
 *  Created on: Apr 24, 2013
 *      Author: luis
 */

#include "ExporterCommGiD.h"

ExporterCommGiD::ExporterCommGiD(
      const SmbData* smb,
      const Comm* comm) : ExporterGiD(smb) {
   comm_ = comm;
   lastSyncing_ = (Real) 0.0;
}

ExporterCommGiD::~ExporterCommGiD() {
   // TODO Auto-generated destructor stub
}

void
ExporterCommGiD::process(
      const Real time,
      const FieldR3& localElec,
      const FieldR3& localMagn) {
//   UInt fSize = comm_->getGlobalSize() * dg_->np;
//   FieldR3 electric(fSize), magnetic(fSize);
//   if ((time != lastSyncing_) || (time == 0.0)) {
//      comm_->gatherFieldsMaster(electric, magnetic, localElec, localMagn);
//      lastSyncing_ = time;
//   }
//   Exporter::process(time, electric, magnetic);
}
