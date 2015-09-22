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
 * OutputRequestFarField.h
 *
 *  Created on: Mar 5, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_

#include "OutRq.h"

class OutRqFarField : public virtual OutRq<Vol> {
public:
    OutRqFarField(const Domain& domain,
                  const string& name,
                  const GroupElements<Vol>& box,
                  const Real iTh, const Real fTh, const Real sTh,
                  const Real iPhi, const Real fPhi, const Real sPhi);
    OutRqFarField(const OutRqFarField& rhs);
    ~OutRqFarField();

    DEFINE_CLONE(OutRqFarField);

    bool hasSameProperties(const OutRqBase& rhs) const;

    Real getInitialTheta() const;
    Real getFinalTheta() const;
    Real getStepTheta() const;
    Real getInitialPhi() const;
    Real getFinalPhi() const;
    Real getStepPhi() const;

private:
    Real initialTheta_, finalTheta_, stepTheta_;
    Real initialPhi_, finalPhi_, stepPhi_;

    void setThetaAndPhi(Real iTh, Real fTh, Real sTh,
                        Real iPhi, Real fPhi, Real sPhi);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_ */
