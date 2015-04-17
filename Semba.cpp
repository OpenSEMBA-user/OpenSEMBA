/*
 * Semba.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: luis
 */

#include "Semba.h"
#include "SolverFDTD.h"
#include "ParserGiD.h"

Semba::Semba(const Arguments& arg) {
    ParserGiD parserGiD(arg.getFilename());
    SmbData* smb = parserGiD.read();
    smb->mesherOptions->set(arg);
    smb->solverOptions->set(arg);

    Solver* solver;
    switch (smb->solverOptions->getSolver()) {
    case OptionsSolver::Solver::ugrfdtd:
        solver = new SolverFDTD(smb);
        solver->run();
        break;
    case OptionsSolver::Solver::cudg3d:
        break;
    default:
        solver = new SolverFDTD(smb);
    }
}

Semba::~Semba() {
    // TODO Auto-generated destructor stub
}

