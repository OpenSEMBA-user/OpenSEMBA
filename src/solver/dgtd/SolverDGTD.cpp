
#ifndef SIMULATION_H_
#include "SolverDGTD.h"
#endif

SolverCudg3d::SolverCudg3d(
      SmbData* smb,
      const OptionsSolverDGTD* arg) {
   arg_ = arg;
   smb_ = smb;
   init();
}

SolverCudg3d::SolverCudg3d(const OptionsSolverDGTD* arg) {
   arg_ = arg;
   smb_ = NULL;
   init();
}

SolverCudg3d::SolverCudg3d(SmbData* smb) {
   arg_ = new OptionsSolverDGTD();
   smb_ = smb;
   init();
}

SolverCudg3d::~SolverCudg3d() {
   delete out_;
   delete dg_;
}

void
SolverCudg3d::init() {

   comm_ = initMPI();
   initOpenMP();

   if (!arg_->getFilename().empty() && smb_ == NULL) {
      ParserGiD parser(arg_->getFilename(), arg_->getProblemTypePath());
      smb_ = parser.read();
   }

   if (arg_->isLinearizeMesh()) {
      smb_->mesh->linearize();
   }
   if (arg_->isPmlIgnore()) {
      smb_->ignorePMLs();
   } else {
      smb_->detectAndAssignPMLRegions();
   }

   integrator_ = initIntegrator(smb_->mesh, smb_->pMGroup, arg_);
   integrator_->partitionate(smb_->mesh, comm_);

   dg_ = new DGExplicit(smb_, comm_, arg_);
   integrator_->setSolver(dg_);

#ifndef USE_MPI
   out_ = new ExporterGiD(smb_, dg_);
#else
   if (comm_->isMaster()) {
      out_ = new OutputCommGiD(smb_, dg_, comm_);
   } else {
      out_ = new OutputComm(smb_, dg_, comm_);
   }
#endif
}

void
SolverCudg3d::run() {
   if (arg_->isDontRun()) {
      return;
   }
   double tSum = 0.0;
   double tRunning = 0.0;
   double time = 0.0;
   uint NtSteps = smb_->gData->finalTime / integrator_->getMaxDT();
   const double dt = integrator_->getMaxDT();
   assert(dt != 0.0);
   while (time < smb_->gData->finalTime) {
      out_->process(time, *dg_->getElectric(), *dg_->getMagnetic());
      double initCPUTime = storeCPUTime();
      integrator_->timeIntegrate(time);
      tSum += storeCPUTime() - initCPUTime;
      time += dt;
      printTimeProfilingInfo(tSum, tRunning, time / dt, NtSteps);
   }
}

Integrator*
SolverCudg3d::initIntegrator(
      const MeshVolume* mesh,
      const PMGroup* pMGroup,
      const OptionsSolverDGTD* arg) {
   Integrator* res;
   switch (arg->getTimeIntegrator()) {
   case OptionsSolverDGTD::lserk4:
      cout<< "- Initializing LSERK Integrator." << endl;
      res = new IntegratorLSERK(*mesh, *pMGroup, arg);
      break;
   case OptionsSolverDGTD::lf2:
      cout<< "- Initializing LF2 Integrator." << endl;
      res = new IntegratorLF2(*mesh, *pMGroup, arg);
      break;
   case OptionsSolverDGTD::lf2full:
      cout<< "- Initializing LF2Full Integrator." << endl;
      res = new IntegratorLF2Full(*mesh, *pMGroup, arg);
      break;
   case OptionsSolverDGTD::verlet:
      cout<< "- Initializing Verlet Integrator." << endl;
      res = new IntegratorVerlet(*mesh, *pMGroup, arg);
      break;
   default:
      cerr << endl << "ERROR@Simulation::buildSolverInfo: "
      << "Undefined time integrator."<< endl;
   }
   return res;
}

Comm* SolverCudg3d::initMPI() {
#ifdef USE_MPI
   return new CommMPI();
#else
   return new CommNone();
#endif
}

void SolverCudg3d::initOpenMP() {
   int nTasksOnThisHost = comm_->getNumOfTasksOnThisHost();
   int maxThreads = omp_get_max_threads() / nTasksOnThisHost - 1;
   if (maxThreads == 0) {
      maxThreads = 1;
   }
   omp_set_num_threads(maxThreads);
}

