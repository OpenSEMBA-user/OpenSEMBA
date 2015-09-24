// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
    if (mesh != NULL) {
        delete mesh;
    }
    if (grid != NULL) {
        delete grid;
    }
    if (solverOptions != NULL) {
        delete solverOptions;
    }
    if (pMGroup != NULL) {
        delete pMGroup;
    }
    if (emSources != NULL) {
        delete emSources;
    }
    if (outputRequests != NULL) {
        delete outputRequests;
    }
    if (mesherOptions != NULL) {
        delete mesherOptions;
    }
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
