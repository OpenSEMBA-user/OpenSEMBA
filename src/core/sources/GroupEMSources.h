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
 * GroupEMSource.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_
#define COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_

#include "../sources/Dipole.h"
#include "../sources/EMSource.h"
#include "../sources/Generator.h"
#include "../sources/PlaneWave.h"
#include "../sources/SourceOnLine.h"
#include "../sources/ports/PortWaveguideRectangular.h"
#include "../sources/ports/PortTEMCoaxial.h"

template<typename E = EMSourceBase>
class GroupEMSources : public Group<E> {
public:
    GroupEMSources() {}
    template<typename E2>
    GroupEMSources(E2* e)                     : Group<E>(e) {}
    template<typename E2>
    GroupEMSources(const std::vector<E2*>& e) : Group<E>(e) {}
    template<typename E2>
    GroupEMSources(VectorPtr<E2>&       rhs) : Group<E>(rhs) {}
    template<typename E2>
    GroupEMSources(const VectorPtr<E2>& rhs) : Group<E>(rhs) {}
    GroupEMSources(VectorPtr<E>&        rhs) : Group<E>(rhs) {}
    template<typename E2>
    GroupEMSources(VectorPtr<E2>&& rhs) : Group<E>(std::move(rhs)) {}
    GroupEMSources(VectorPtr<E >&& rhs) : Group<E>(std::move(rhs)) {}
    virtual ~GroupEMSources() {}

    DEFINE_GROUP_CLONE(GroupEMSources, E);

    GroupEMSources& operator=(VectorPtr<E>&);
    GroupEMSources& operator=(VectorPtr<E>&&);

    bool isSimilar(const GroupEMSources& rhs) const;
    bool check() const;

    void printInfo() const;
};

#include "GroupEMSources.hpp"

typedef GroupEMSources<> EMSourceGroup;

#endif /* COMMON_ELECTROMAGNETICSOURCES_GROUPEMSOURCES_H_ */
