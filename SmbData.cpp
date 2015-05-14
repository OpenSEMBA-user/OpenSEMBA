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
    if (rhs.mesh != NULL) {
        mesh = rhs.mesh->cloneTo<Mesh>();
    } else {
        mesh == NULL;
    }
    if (rhs.grid != NULL) {
        grid = rhs.grid->cloneTo<Grid3>();
    } else {
        grid == NULL;
    }
    if (rhs.solverOptions != NULL) {
        solverOptions = rhs.solverOptions->cloneTo<OptionsSolver>();
    } else {
        solverOptions == NULL;
    }
    if (rhs.pMGroup != NULL) {
        pMGroup = rhs.pMGroup->cloneTo<GroupPhysicalModels<>>();
    } else {
        pMGroup == NULL;
    }
    if (rhs.emSources != NULL) {
        emSources = rhs.emSources->cloneTo<GroupEMSources<>>();
    } else {
        emSources == NULL;
    }
    if (rhs.outputRequests != NULL) {
        outputRequests = rhs.outputRequests->cloneTo<GroupOutRqs<>>();
    } else {
        outputRequests == NULL;
    }
    if (rhs.mesherOptions != NULL) {
        mesherOptions = rhs.mesherOptions->cloneTo<OptionsMesher>();
    } else {
        mesherOptions == NULL;
    }
}

SmbData::~SmbData() {

}

SmbData& SmbData::operator=(const SmbData& rhs) {
    if (this == &rhs) {
        return *this;
    }
    ProjectFile::operator =(rhs);
    mesh = rhs.mesh->cloneTo<Mesh>();
    grid = rhs.grid->cloneTo<Grid3>();
    solverOptions = rhs.solverOptions->cloneTo<OptionsSolver>();
    pMGroup = rhs.pMGroup->cloneTo<GroupPhysicalModels<>>();
    emSources = rhs.emSources->cloneTo<GroupEMSources<>>();
    outputRequests = rhs.outputRequests->cloneTo<GroupOutRqs<>>();
    mesherOptions = rhs.mesherOptions->cloneTo<OptionsMesher>();
    return *this;
}

void SmbData::applyGeometricScalingFactor() {

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
