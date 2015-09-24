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
#ifndef GROUPID_H_
#define GROUPID_H_

#include <iostream>
#include <map>
#include <sstream>

#include "../error/Error.h"

#include "Group.h"

template<typename T, class Id>
class GroupId : public Group<T> {
public:
    class ErrorId : public virtual Error {
    public:
        ErrorId(const Id&);
        virtual ~ErrorId() throw();

        Id getId() const;
    private:
        Id id_;
    };
    class ErrorIdNotExists : public ErrorId {
    public:
        ErrorIdNotExists(const Id&);
        virtual ~ErrorIdNotExists() throw();
    };
    class ErrorIdZero : public ErrorId {
    public:
        ErrorIdZero(const Id&);
        virtual ~ErrorIdZero() throw();
    };
    class ErrorIdDuplicated : public ErrorId {
    public:
        ErrorIdDuplicated(const Id&);
        virtual ~ErrorIdDuplicated() throw();
    };

    GroupId();
    template<typename T2>
    GroupId(T2*);
    template<typename T2>
    GroupId(const std::vector<T2*>&);
    template<typename T2>
    GroupId(VectorPtr<T2>&);
    template<typename T2>
    GroupId(const VectorPtr<T2>&);
    GroupId(VectorPtr<T>&);
    template<typename T2>
    GroupId(VectorPtr<T2>&&);
    GroupId(VectorPtr<T>&&);
    virtual ~GroupId();

    GroupId<T,Id>* clone() const;

    GroupId<T,Id>& operator=(VectorPtr<T>&);
    GroupId<T,Id>& operator=(VectorPtr<T>&&);

    virtual void clear();

    bool existId(const Id id) const;

    std::vector<Id> getIds() const;

    virtual typename Reference<T*>::type      getId(const Id id);
    virtual typename ConstReference<T*>::type getId(const Id id) const;
    virtual GroupId<T,Id>       getId(const std::vector<Id>&);
    virtual GroupId<const T,Id> getId(const std::vector<Id>&) const;

    VectorPtr<T> add(VectorPtr<T>&);
    VectorPtr<T> add(VectorPtr<T>&&);
    using VectorPtr<T>::add;

    template<typename T2>
    VectorPtr<T> addId(T2*);
    template<typename T2>
    VectorPtr<T> addId(const std::vector<T2*>&);
    template<typename T2>
    VectorPtr<T> addId(VectorPtr<T2>&);
    VectorPtr<T> addId(VectorPtr<T>&);
    VectorPtr<T> addId(VectorPtr<T>&&);

    virtual void remove(const UInt&);
    virtual void remove(const std::vector<UInt>&);

    virtual void removeId(const Id);
    virtual void removeId(const std::vector<Id>&);

private:
    Id lastId_;
    std::map<Id, UInt> mapId_;

    void postprocess_(const UInt& pos);
    std::vector<UInt> getElemsId_(const std::vector<Id>&) const;
};

#include "GroupId.hpp"

#endif /* GROUPID_H_ */
