/*
 * SmbData.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#include "SmbData.h"

SmbData::SmbData() {
    mesh = NULL;
    grid = NULL;
    solverOptions = NULL;
    pMGroup = NULL;
    emSources = NULL;
    outputRequests = NULL;
    mesherOptions = NULL;
}

SmbData::SmbData(const SmbData& rhs) {
    mesh = rhs.mesh->clone()->castTo<Mesh>();
    grid = new Grid3(*rhs.grid);
    solverOptions = new SolverOptions(*rhs.solverOptions);
    pMGroup = new PhysicalModelGroup<>(*rhs.pMGroup);
    emSources = new EMSourceGroup<>(*rhs.emSources);
    outputRequests = new OutRqGroup<>(*rhs.outputRequests);
    mesherOptions = new MesherOptions(*rhs.mesherOptions);
}

SmbData::~SmbData() {

}

ClassBase* SmbData::clone() const {
    return new SmbData(*this);
}

SmbData& SmbData::operator=(const SmbData& rhs) {
    if (this == &rhs) {
        return *this;
    }
    mesh = rhs.mesh->clone()->castTo<Mesh>();
    grid = new Grid3(*rhs.grid);
    solverOptions = new SolverOptions(*rhs.solverOptions);
    pMGroup = new PhysicalModelGroup<>(*rhs.pMGroup);
    emSources = new EMSourceGroup<>(*rhs.emSources);
    outputRequests = new OutRqGroup<>(*rhs.outputRequests);
    mesherOptions = new MesherOptions(*rhs.mesherOptions);
    return *this;
}

void SmbData::applyScalingFactor() {

    mesh->applyScalingFactor(mesherOptions->getScalingFactor());
    mesherOptions->applyGeometricScalingFactor(mesherOptions->getScalingFactor());
    mesherOptions->setScalingFactor((Real) 1.0);
}

void SmbData::printInfo() const {
    cout << " --- SEMBA data --- " << endl;
    if (mesh != NULL) {
        mesh->printInfo();
    } else {
        cout << "No info about mesh." << endl;
    }
    if (solverOptions != NULL) {
        solverOptions->printInfo();
    } else {
        cout << "No info about global data." << endl;
    }
    if (pMGroup != NULL) {
        pMGroup->printInfo();
    } else {
        cout << "No info about physical models." << endl;
    }
    if (emSources != NULL) {
        emSources->printInfo();
    } else {
        cout << "No info about electromagnetic sources." << endl;
    }
    if (outputRequests != NULL) {
        outputRequests->printInfo();
    } else {
        cout << "No info about output requests." << endl;
    }
    if (mesherOptions != NULL) {
        mesherOptions->printInfo();
    } else {
        cout << "No info about mesher options." << endl;
    }
}
