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
 * MagnitudeNumerical.h
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_

#include <cassert>
#include <cmath>
#include <map>
using namespace std;

#include "ProjectFile.h"
#include "math/FunctionLinearInterpolation.h"
#include "base/error/ErrorFile.h"
#include "base/error/ErrorNotImplemented.h"

#include "../../sources/magnitude/Magnitude.h"

class MagnitudeNumerical : public virtual Magnitude,
                           public virtual ProjectFile {
public:
    MagnitudeNumerical();
    MagnitudeNumerical(const ProjectFile& filename);
    MagnitudeNumerical(const ProjectFile& filename,
                       const Magnitude& mag,
                       const Real timeStep,
                       const Real finalTime);
    virtual ~MagnitudeNumerical();

    DEFINE_CLONE(MagnitudeNumerical);

    MagnitudeNumerical& operator=(const MagnitudeNumerical& rhs);

    bool operator==(const Magnitude&) const;
    Real evaluate(const Real time) const;

    void printInfo() const;

private:
    static const UInt defaultNumberOfSteps = 1000;


};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDENUMERICAL_H_ */
