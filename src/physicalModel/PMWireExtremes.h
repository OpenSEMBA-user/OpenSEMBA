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
 * PMWireExtremes.h
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMWIREEXTREMES_H_
#define SRC_COMMON_PHYSICALMODEL_PMWIREEXTREMES_H_

#include "PMWire.h"
#include "PMMultiport.h"

class PMWireExtremes: public PMWire {
public:
    PMWireExtremes(const PMWire& wire,
                   const PMMultiport* extremeL,
                   const PMMultiport* extremeR);
    PMWireExtremes(const PMWireExtremes& rhs);
    virtual ~PMWireExtremes();

    DEFINE_CLONE(PMWireExtremes);

    const PMMultiport *getExtreme(const UInt i) const { return extreme_[i]; }

    void setExtreme(const UInt i, const PMMultiport* extreme);

    void printInfo() const;

private:
    const PMMultiport* extreme_[2];
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMWIREEXTREMES_H_ */
