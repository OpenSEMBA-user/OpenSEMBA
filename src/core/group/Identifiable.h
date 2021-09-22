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

#pragma once

#include <exception>
#include <map>

#include "Group.h"

namespace SEMBA {
namespace Group {

template<typename T, class Id>
class Identifiable : public Group<T> {
public:
    Identifiable();
    template<typename T2>
    Identifiable(T2*);
    template<typename T2>
    Identifiable(const std::vector<T2*>&);
    template<typename T2>
    Identifiable(Group<T2>&);
    template<typename T2>
    Identifiable(const Group<T2>&);
    Identifiable(Group<T>&);
    template<typename T2>
    Identifiable(Group<T2>&&);
    Identifiable(Group<T>&&);
    virtual ~Identifiable();

    Identifiable<T,Id>& operator=(Group<T>&);
    Identifiable<T,Id>& operator=(Group<T>&&);

    virtual void clear();

    bool existId(const Id id) const;

    std::vector<Id> getIds() const;

    virtual T*       getId(const Id id);
    virtual const T* getId(const Id id) const;
    virtual Identifiable<T,Id>       getId(const std::vector<Id>&);
    virtual Identifiable<const T,Id> getId(const std::vector<Id>&) const;

    using Group<T>::add;
    Group<T> add(Group<T>&);
    Group<T> add(Group<T>&&);

    template<typename T2>
    Group<T> addId(T2*);
    template<typename T2>
    Group<T> addId(const std::vector<T2*>&);
    template<typename T2>
    Group<T> addId(Group<T2>&);
    Group<T> addId(Group<T>&);
#ifdef __GNUC__
    Group<T> addId(Group<T>&&);
#endif

    virtual void remove(const std::size_t&);
    virtual void remove(const std::vector<std::size_t>&);

    virtual void removeId(const Id);
    virtual void removeId(const std::vector<Id>&);

private:
    Id lastId_;
    std::map<Id, std::size_t> mapId_;

    void postprocess_(const std::size_t& pos);
    std::vector<std::size_t> getElemsId_(const std::vector<Id>&) const;
};

} /* namespace Group */
} /* namespace SEMBA */

#include "Identifiable.hpp"

