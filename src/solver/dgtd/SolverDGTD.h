// File: simulation.h
#ifndef SIMULATION_H_
#	define SIMULATION_H_

#include "parser/gid/ParserGiD.h"
#include "exporter/gid/ExporterGiD.h"
#include "options/OptionsSolverDGTD.h"
#include "physicalModel/GroupPhysicalModels.h"
#ifdef USE_MPI
    #include "CommMPI.h"
    #include "../output/OutputCommGiD.h"
    #include "../output/OutputComm.h"
#else
    #include "CommNone.h"
#endif
#include "../integrator/IntegratorLSERK.h"
#include "../integrator/IntegratorLF2.h"
#include "../integrator/IntegratorLF2Full.h"
#include "../integrator/IntegratorVerlet.h"
#include "solver/Solver.h"
#include "../DG/DGExplicit.h"

class SolverCudg3d : public Solver {
public:
    SolverCudg3d(SmbData*);
    ~SolverCudg3d();
    bool run();
private:
    SmbData* smb_;
    Comm *comm_;
    Integrator *integrator_;
    DG *dg_;
    Exporter* out_;
    Integrator* initIntegrator(
            const MeshVolume* mesh,
            const PMGroup* pMGroup,
            const OptionsSolverDGTD* args);
    Comm* initMPI();
    void initOpenMP();
};

#endif
