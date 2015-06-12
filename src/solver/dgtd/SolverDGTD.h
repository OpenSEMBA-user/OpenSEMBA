// File: simulation.h
#ifndef SIMULATION_H_
#	define SIMULATION_H_

#include "parser/gid/ParserGiD.h"
#include "../output/OutputGiD.h"
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
#include "../Solver.h"
#include "../DG/DGExplicit.h"
#include "../DG/DGLowMemory.h"

class SolverCudg3d : public Solver {
public:
    SolverCudg3d(SmbData*);
    SolverCudg3d(const OptionsSolverDGTD*);
    SolverCudg3d(SmbData*, const OptionsSolverDGTD*);
    ~SolverCudg3d();
    void
    run();
private:
    const OptionsSolverDGTD* arg_;
    SmbData* smb_;
    Comm *comm_;
    Integrator *integrator_;
    DG *dg_;
    Exporter* out_;
    Integrator* initIntegrator(
            const MeshVolume* mesh,
            const GroupPhysicalModels* pMGroup,
            const OptionsSolverDGTD* args);
    Comm* initMPI();
    void initOpenMP();
    void init();
};

#endif
