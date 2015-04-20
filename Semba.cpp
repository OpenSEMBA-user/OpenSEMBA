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

    switch (smb->solverOptions->getSolver()) {
    case OptionsSolver::Solver::ugrfdtd:
        solver_ = new SolverFDTD(smb);
        break;
    case OptionsSolver::Solver::cudg3d:
        break;
    default:
        solver_ = new SolverFDTD(smb);
    }
}

Semba::~Semba() {
    // TODO Auto-generated destructor stub
}

bool Semba::run() {
    return solver_->run();
}

bool Semba::canRun() const {
    return solver_->canRun();
}
