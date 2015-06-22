/*
 * CommOutput.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: luis
 */
//
//#include "ExporterComm.h"
//
//ExporterComm::ExporterComm(
//      const SmbData* smb,
//      const Comm* comm) :
//      Output(smb, dg) {
//   comm_ = comm;
//   lastSyncing_ = (Real) 0.0;
//}
//
//ExporterComm::~ExporterComm() {
//
//}
//
//void
//ExporterComm::writeResults(
//      const FieldR3& electric,
//      const FieldR3& magnetic,
//      CVecR3& EInc,
//      const Real time, const UInt tStep) {
//   if ((time != lastSyncing_) || (tStep == 0)) {
//      comm_->gatherFieldsSlave(electric, magnetic);
//      lastSyncing_ = time;
//   }
//}
