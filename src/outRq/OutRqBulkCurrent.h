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
 * OutRqBulkCurrent.h
 *
 *  Created on: 20 de ago. de 2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_OUTRQ_OUTRQBULKCURRENT_H_
#define SRC_COMMON_OUTRQ_OUTRQBULKCURRENT_H_

#include "OutRq.h"

class OutRqBulkCurrent : public virtual OutRq<Elem> {
public:
    OutRqBulkCurrent(const Domain& domain,
                     const string& name,
                     const GroupElements<Elem>& box,
                     const CartesianAxis& dir,
                     const UInt& skip);
    OutRqBulkCurrent(const OutRqBulkCurrent& rhs);
    virtual ~OutRqBulkCurrent();

    DEFINE_CLONE(OutRqBulkCurrent);

    bool hasSameProperties(const OutRqBase& rhs) const;

    CartesianAxis getDir() const;
    UInt          getSkip() const;

private:
    CartesianAxis dir_;
    UInt skip_;
};

#endif /* SRC_COMMON_OUTRQ_OUTRQBULKCURRENT_H_ */
