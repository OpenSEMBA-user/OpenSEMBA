#include "SolverDGTD.h"

SolverCudg3d::SolverCudg3d(SmbData* smb) {
    const MeshVolume* mesh = smb_->mesh->castTo<MeshVolume>();
    comm_ = initMPI();
    initOpenMP();

    OptionsSolverDGTD* arg = smb_->solverOptions->castTo<OptionsSolverDGTD>();
    integrator_ = initIntegrator(mesh, smb_->pMGroup, arg);
    integrator_->partitionate(mesh, comm_);

    dg_ = new DGExplicit(smb_, comm_);
    integrator_->setSolver(dg_);

#ifndef USE_MPI
    out_ = new ExporterGiD(smb_);
#else
    if (comm_->isMaster()) {
        out_ = new OutputCommGiD(smb_, dg_, comm_);
    } else {
        out_ = new OutputComm(smb_, dg_, comm_);
    }
#endif
}

SolverCudg3d::~SolverCudg3d() {
    delete out_;
    delete dg_;
}

bool SolverCudg3d::run() {
    Real tSum = 0.0;
    Real tRunning = 0.0;
    Real time = 0.0;
    const Real dt = integrator_->getMaxDT();
    assert(dt != 0.0);
    while (time < smb_->solverOptions->getFinalTime()) {
        out_->process(time, *dg_->getElectric(), *dg_->getMagnetic());
        Real initCPUTime = storeCPUTime();
        integrator_->timeIntegrate(time);
        tSum += storeCPUTime() - initCPUTime;
        time += dt;
        printTimeProfilingInfo(tSum, tRunning, time / dt,
                smb_->solverOptions->getNumberOfTimeSteps());
    }
    return true;
}

Integrator* SolverCudg3d::initIntegrator(
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
    Int nTasksOnThisHost = comm_->getNumOfTasksOnThisHost();
    Int maxThreads = omp_get_max_threads() / nTasksOnThisHost - 1;
    if (maxThreads == 0) {
        maxThreads = 1;
    }
    omp_set_num_threads(maxThreads);
}
