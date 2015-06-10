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

SmbData::SmbData(const SmbData& rhs)
:   ProjectFile(rhs) {
    mesh = NULL;
    grid = NULL;
    solverOptions = NULL;
    pMGroup = NULL;
    emSources = NULL;
    outputRequests = NULL;
    mesherOptions = NULL;
    if (rhs.mesh != NULL) {
        mesh = rhs.mesh->cloneTo<Mesh>();
    }
    if (rhs.grid != NULL) {
        grid = rhs.grid->cloneTo<Grid3>();
    }
    if (rhs.solverOptions != NULL) {
        solverOptions = rhs.solverOptions->cloneTo<OptionsSolver>();
    }
    if (rhs.pMGroup != NULL) {
        pMGroup = rhs.pMGroup->cloneTo<GroupPhysicalModels<>>();
    }
    if (rhs.emSources != NULL) {
        emSources = rhs.emSources->cloneTo<GroupEMSources<>>();
    }
    if (rhs.outputRequests != NULL) {
        outputRequests = rhs.outputRequests->cloneTo<GroupOutRqs<>>();
    }
    if (rhs.mesherOptions != NULL) {
        mesherOptions = rhs.mesherOptions->cloneTo<OptionsMesher>();
    }
}

SmbData::~SmbData() {

}

SmbData& SmbData::operator=(const SmbData& rhs) {
    if (this == &rhs) {
        return *this;
    }
    ProjectFile::operator=(rhs);
    mesh = NULL;
    grid = NULL;
    solverOptions = NULL;
    pMGroup = NULL;
    emSources = NULL;
    outputRequests = NULL;
    mesherOptions = NULL;
    if (rhs.mesh != NULL) {
        mesh = rhs.mesh->cloneTo<Mesh>();
    }
    if (rhs.grid != NULL) {
        grid = rhs.grid->cloneTo<Grid3>();
    }
    if (rhs.solverOptions != NULL) {
        solverOptions = rhs.solverOptions->cloneTo<OptionsSolver>();
    }
    if (rhs.pMGroup != NULL) {
        pMGroup = rhs.pMGroup->cloneTo<GroupPhysicalModels<>>();
    }
    if (rhs.emSources != NULL) {
        emSources = rhs.emSources->cloneTo<GroupEMSources<>>();
    }
    if (rhs.outputRequests != NULL) {
        outputRequests = rhs.outputRequests->cloneTo<GroupOutRqs<>>();
    }
    if (rhs.mesherOptions != NULL) {
        mesherOptions = rhs.mesherOptions->cloneTo<OptionsMesher>();
    }
    return *this;
}

void SmbData::printInfo() const {
    cout << " --- SEMBA data --- " << endl;
    if (grid != NULL) {
        grid->printInfo();
    }
    if (mesh != NULL) {
        mesh->printInfo();
    } else {
        cout << "No info about mesh." << endl;
    }
    if (mesherOptions != NULL) {
        mesherOptions->printInfo();
    } else {
        cout << "No info about mesher options." << endl;
    }
    if (solverOptions != NULL) {
        solverOptions->printInfo();
    } else {
        cout << "No info about solver options." << endl;
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
}

bool SmbData::check() const {
    bool res = true;
    if (emSources != NULL) {
        res &= emSources->check();
    }
    if (outputRequests != NULL) {
        //    res &= outputRequests->check();
    }
    return res;
}
