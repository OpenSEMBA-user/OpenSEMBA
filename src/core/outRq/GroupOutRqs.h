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
#ifndef COMMON_OUTRQ_GROUPOUTRQS_H_
#define COMMON_OUTRQ_GROUPOUTRQS_H_

#include "OutRq.h"
#include "OutRqBulkCurrent.h"
#include "OutRqFarField.h"

template<typename O = OutRqBase>
class GroupOutRqs : public Group<O> {
public:
    GroupOutRqs() {}
    template<typename O2>
    GroupOutRqs(O2* e)                     : Group<O>(e) {}
    template<typename O2>
    GroupOutRqs(const std::vector<O2*>& e) : Group<O>(e) {}
    template<typename O2>
    GroupOutRqs(VectorPtr<O2>&       rhs) : Group<O>(rhs) {}
    template<typename O2>
    GroupOutRqs(const VectorPtr<O2>& rhs) : Group<O>(rhs) {}
    GroupOutRqs(VectorPtr<O>&        rhs) : Group<O>(rhs) {}
    template<typename O2>
    GroupOutRqs(VectorPtr<O2>&& rhs) : Group<O>(std::move(rhs)) {}
    GroupOutRqs(VectorPtr<O >&& rhs) : Group<O>(std::move(rhs)) {}
    virtual ~GroupOutRqs() {}

    DEFINE_GROUP_CLONE(GroupOutRqs, O);

    GroupOutRqs& operator=(VectorPtr<O>&);
    GroupOutRqs& operator=(VectorPtr<O>&&);

    using Group<O>::add;
    template<typename O2>
    VectorPtr<O> add(O2* newElem);
    template<typename O2>
    vector<O*> add(vector<O2*>&);

    bool isSimilar(const GroupOutRqs& rhs) const;

    void printInfo() const;
};

#include "GroupOutRqs.hpp"

#endif
