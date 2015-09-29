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

#ifndef BOUNDARYCONDITION_H_
#define BOUNDARYCONDITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include <utility>

using namespace std;

#include "SmbData.h"
#include "CellGroup.h"

class BoundaryCondition {
public:
    const CellTet<ORDER_N>* cell;
    uint face;
    const Condition* condition;
    //
    BoundaryCondition();
    virtual ~BoundaryCondition();
    bool isSMA() const {return condition->isSMA();}
    virtual bool isPEC() const {return condition->isPEC();}
    virtual bool isPMC() const {return condition->isPMC();}
    virtual bool isSurfaceImpedance() const {
        return condition->isSurfaceImpedance();
    }
    bool hasSameBoundary(const BoundaryCondition& other) const {
        return (cell == other.cell && face == other.face);
    }
    virtual const CellTet<ORDER_N>* getCellD() const;
    virtual uint getFaceD() const;
    virtual void printInfo() const;
};

class EMSourceBC : public BoundaryCondition {
public:
    //
    EMSourceBC();
    virtual ~EMSourceBC();
    EMSourceBC(const BoundaryCondition& param);
    EMSourceBC(const CellTet<ORDER_N>* e, const uint f, const Condition* bc);
    EMSourceBC& operator=(const EMSourceBC& rhs);
    void check() const;
};

class PhysicalModelBC : public BoundaryCondition {
public:
    //
    PhysicalModelBC();
    PhysicalModelBC(const BoundaryCondition& param);
    PhysicalModelBC(
            const CellTet<ORDER_N>*,
            uint face,
            const Condition* bc);
    virtual ~PhysicalModelBC();
    PhysicalModelBC& operator=(const PhysicalModelBC& rhs);
    bool
    checkIsPMValidForSurfaces() const;
};

class SurfaceImpedanceBC : public BoundaryCondition {
public:
    const CellTet<ORDER_N>* cellD;
    uint faceD;
    //
    SurfaceImpedanceBC();
    virtual ~SurfaceImpedanceBC();
    SurfaceImpedanceBC(
            const CellTet<ORDER_N>* cell,
            const uint face,
            const CellTet<ORDER_N>* cellD,
            const uint faceD,
            const Condition* cond);
    SurfaceImpedanceBC& operator=(const SurfaceImpedanceBC &rhs);
    bool isSIBC() const;
    const CellTet<ORDER_N>* getCellD() const;
    uint getFaceD() const;
};

#endif /* BOUNDARYCONDITION_H_ */
