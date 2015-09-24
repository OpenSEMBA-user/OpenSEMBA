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
 * OpenFOAMParameters.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef OPENFOAMPARAMETERS_H_
#define OPENFOAMPARAMETERS_H_

#include <cstdlib>
#include <string>
#include <array>

using namespace std;

#include "math/CartesianVector.h"
#include "Options.h"

class OptionsMesher : public Options {
public:
    typedef enum {
        openfoam,
        ugrMesher,
        zMesher,
        none
    } Mesher;
    typedef enum {
        structured,
        relaxed,
        slanted,
        conformal
    } Mode;
    typedef enum {
        pec,
        pmc,
        pml,
        mur1,
        mur2,
        periodic,
        symmetricETan,
        symmetricHTan,
        undefined
    } BoundType;
    OptionsMesher();

    DEFINE_CLONE(OptionsMesher);

    virtual void set(const Arguments& args);
    void setBoundaryMeshSize(const pair<CVecR3, CVecR3>& boundaryMeshSize);
    void setBoundTermination(const UInt i, UInt j, BoundType bound);
    void setBruteForceVolumes(bool bruteForceVolumes);
    void setForbiddenLength(const Real& edgeFraction);
    void setLocationInMesh(const CVecR3& locationInMesh);
    void setLocationInMeshSet(bool locationInMeshSet);
    void setMesher(Mesher mesher);
    void setMode(Mode mode);
    void setScaleFactor(bool scaleFactor);
    void setScaleFactorValue(const string& scaleFactorValue);
    void applyGeometricScalingFactor(const Real& factor);
    void setOutputName(const string& outputName);

    Mesher getMesher() const;
    virtual const CVecR3& getLocationInMesh() const;
    virtual bool isLocationInMeshSet() const;
    Mode getMode() const;
    bool isStructured() const;
    bool isRelaxed() const;
    bool isBruteForceVolumes() const;
    Real getForbiddenLength() const;
    bool hasScaleFactor() const;
    string getScaleFactor() const;
    const string& getMeshOutputName() const;
    bool isGridStepSet() const;
    bool isVtkExport() const;
    bool isPostmshExport() const;
    const CVecR3& getGridStep() const;

    const pair<CVecR3, CVecR3>& getBoundaryMeshSize() const;
    array<pair<BoundType,BoundType>,3> getBoundTerminations() const;
    BoundType getBoundTermination(const UInt i, const UInt p) const;
    const string& getOutputName() const;

    void printHelp() const;
    void printInfo() const;
    static string toStr(const BoundType);
    void setGridStep(const CVecR3& gridStep);
    void setVtkExport(bool vtkExport);
    void setPostmshExport(bool postmshExport);

private:
    Mesher mesher_;
    Mode mode_;
    bool bruteForceVolumes_;
    Real forbiddenLength_;
    bool scaleFactor_;
    string scaleFactorValue_;
    bool locationInMeshSet_;
    CVecR3 locationInMesh_;
    string outputName_;

    bool vtkExport_;
    bool postmshExport_;
    CVecR3 gridStep_;
    array<pair<BoundType,BoundType>,3> boundTermination_;
};

typedef array<pair<OptionsMesher::BoundType,OptionsMesher::BoundType>,3>
        BoundTerminations;
//typedef OptionsMesher ConfOptSwitch;

#endif /* OPENFOAMPARAMETERS_H_ */
