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
 * PMMultilayer.h
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_
#define SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_

#include "base/error/Error.h"

#include "PMSurface.h"

class PMSurfaceMultilayer : public PMSurface {
public:
    class ErrorIncompatibleSizes : public Error {
    public:
        ErrorIncompatibleSizes();
        virtual ~ErrorIncompatibleSizes() throw();
    };

    PMSurfaceMultilayer();
    PMSurfaceMultilayer(
    const MatId id,
    const string& name,
    const vector<Real>& thickness,
    const vector<Real>& relPermittivity,
    const vector<Real>& relPermeability,
    const vector<Real>& elecCond,
    const vector<Real>& magnCond);

    DEFINE_CLONE(PMSurfaceMultilayer);

    UInt getNumberOfLayers() const;
    string printLayer(const UInt i) const;
    Real getThickness(const UInt i) const;
    Real getPermittivity(const UInt i) const;
    Real getPermeability(const UInt i) const;
    Real getElecCond(const UInt i) const;
    Real getMagnCond(const UInt i) const;

    void printInfo() const;
private:
    vector<Real> thickness_, relPermittivity_, relPermeability_,
                 elecCond_, magnCond_;
};

#endif /* SRC_COMMON_INPUTS_PHYSICALMODEL_PMSURFACEMULTILAYER_H_ */
