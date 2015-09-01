/*
 * SmbData.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: luis
 */

#include "SmbData.h"

SmbData::SmbData() {
    mesh = nullptr;
    grid = nullptr;
    solverOptions = nullptr;
    pMGroup = nullptr;
    emSources = nullptr;
    outputRequests = nullptr;
    mesherOptions = nullptr;
}

SmbData::SmbData(const SmbData& rhs)
:   ProjectFile(rhs) {
    mesh = nullptr;
    grid = nullptr;
    solverOptions = nullptr;
    pMGroup = nullptr;
    emSources = nullptr;
    outputRequests = nullptr;
    mesherOptions = nullptr;
    if (rhs.mesh != nullptr) {
        mesh = rhs.mesh->cloneTo<Mesh>();
    }
    if (rhs.grid != nullptr) {
        grid = rhs.grid->cloneTo<Grid3>();
    }
    if (rhs.solverOptions != nullptr) {
        solverOptions = rhs.solverOptions->cloneTo<OptionsSolver>();
    }
    if (rhs.pMGroup != nullptr) {
        pMGroup = rhs.pMGroup->cloneTo<GroupPhysicalModels<>>();
    }
    if (rhs.emSources != nullptr) {
        emSources = rhs.emSources->cloneTo<GroupEMSources<>>();
    }
    if (rhs.outputRequests != nullptr) {
        outputRequests = rhs.outputRequests->cloneTo<GroupOutRqs<>>();
    }
    if (rhs.mesherOptions != nullptr) {
        mesherOptions = rhs.mesherOptions->cloneTo<OptionsMesher>();
    }
}

SmbData::~SmbData() {
    if (mesh != nullptr) {
        delete mesh;
    }
    if (grid != nullptr) {
        delete grid;
    }
    if (solverOptions != nullptr) {
        delete solverOptions;
    }
    if (pMGroup != nullptr) {
        delete pMGroup;
    }
    if (emSources != nullptr) {
        delete emSources;
    }
    if (outputRequests != nullptr) {
        delete outputRequests;
    }
    if (mesherOptions != nullptr) {
        delete mesherOptions;
    }
}

SmbData& SmbData::operator=(const SmbData& rhs) {
    if (this == &rhs) {
        return *this;
    }
    ProjectFile::operator=(rhs);
    mesh = nullptr;
    grid = nullptr;
    solverOptions = nullptr;
    pMGroup = nullptr;
    emSources = nullptr;
    outputRequests = nullptr;
    mesherOptions = nullptr;
    if (rhs.mesh != nullptr) {
        mesh = rhs.mesh->cloneTo<Mesh>();
    }
    if (rhs.grid != nullptr) {
        grid = rhs.grid->cloneTo<Grid3>();
    }
    if (rhs.solverOptions != nullptr) {
        solverOptions = rhs.solverOptions->cloneTo<OptionsSolver>();
    }
    if (rhs.pMGroup != nullptr) {
        pMGroup = rhs.pMGroup->cloneTo<GroupPhysicalModels<>>();
    }
    if (rhs.emSources != nullptr) {
        emSources = rhs.emSources->cloneTo<GroupEMSources<>>();
    }
    if (rhs.outputRequests != nullptr) {
        outputRequests = rhs.outputRequests->cloneTo<GroupOutRqs<>>();
    }
    if (rhs.mesherOptions != nullptr) {
        mesherOptions = rhs.mesherOptions->cloneTo<OptionsMesher>();
    }
    return *this;
}

void SmbData::printInfo() const {
    cout << " --- SEMBA data --- " << endl;
    if (grid != nullptr) {
        grid->printInfo();
    }
    if (mesh != nullptr) {
        mesh->printInfo();
    } else {
        cout << "No info about mesh." << endl;
    }
    if (mesherOptions != nullptr) {
        mesherOptions->printInfo();
    } else {
        cout << "No info about mesher options." << endl;
    }
    if (solverOptions != nullptr) {
        solverOptions->printInfo();
    } else {
        cout << "No info about solver options." << endl;
    }
    if (pMGroup != nullptr) {
        pMGroup->printInfo();
    } else {
        cout << "No info about physical models." << endl;
    }
    if (emSources != nullptr) {
        emSources->printInfo();
    } else {
        cout << "No info about electromagnetic sources." << endl;
    }
    if (outputRequests != nullptr) {
        outputRequests->printInfo();
    } else {
        cout << "No info about output requests." << endl;
    }
}

bool SmbData::check() const {
    bool res = true;
    if (emSources != nullptr) {
        res &= emSources->check();
    }
    if (outputRequests != nullptr) {
        //    res &= outputRequests->check();
    }
    return res;
}
