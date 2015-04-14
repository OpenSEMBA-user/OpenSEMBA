// File: simulation.h
/* =============== Simulation handling procedures =============================
   PURPOSE:
   - The simulation objects handle the input arguments provided when the app is
     called.
   - Contains all other data.
   ========================================================================= */
#ifndef SIMULATION_H_
#	define SIMULATION_H_

#include "ArgumentsCudg3d.h"
#include "../../../common/parser/ParserGiD.h"
#include "../output/OutputGiD.h"
#ifdef USE_AHDF_API
	#include "../parser/ParserAHDF.h"
	#include "../output/OutputAHDF.h"
#endif
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
	Output* out_;
	Integrator* initIntegrator(
	  const MeshVolume* mesh,
	  const PhysicalModelGroup* pMGroup,
	  const OptionsSolverDGTD* args);
	Comm* initMPI();
	void initOpenMP();
	void init();
};

#endif
