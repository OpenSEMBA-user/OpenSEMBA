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

#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif

#include <cstddef>
#include <memory>
#include <typeinfo>
#include <vector>

namespace SEMBA {
namespace Group {

template <typename T>
class Base {
public:
    Base();
    virtual ~Base();

    virtual std::size_t size () const = 0;
    virtual bool        empty() const;

    T*       operator()(const std::size_t pos);
    const T* operator()(const std::size_t pos) const;

    virtual T*       get(const std::size_t pos) = 0;
    virtual const T* get(const std::size_t pos) const = 0;
};

template<typename T>
class Group : public virtual Base<T> {
public:
    Group() {}
    template<typename T2>
    Group(T2*);
    template<typename T2>
    Group(const std::vector<T2*>&);
    template<typename T2>
    Group(Group<T2>&);
    template<typename T2>
    Group(Group<T2>&&);
    template<typename T2>
    Group(const Group<T2>&);
    Group(Group&);
    Group(Group&&);
    virtual ~Group() {}

    virtual Group& operator=(Group&  rhs);
    virtual Group& operator=(Group&& rhs);

    void resize (const std::size_t nS) { element_.resize (nS); }
    void reserve(const std::size_t nS) { element_.reserve(nS); }
    virtual void clear()               { element_.clear();     }

    bool        empty() const { return element_.empty(); }
    std::size_t size () const { return element_.size();  }

    template<class T2>
    bool        emptyOf() const;
    template<class T2>
    std::size_t sizeOf () const;

    T*       get(const std::size_t i);
    const T* get(const std::size_t i) const;
    Group<T>       get(const std::vector<std::size_t>&);
    Group<const T> get(const std::vector<std::size_t>&) const;

    template<class T2>
    Group<typename std::conditional<std::is_const<T>::value,
                                    const T2, T2>::type>
                    getOf();
    template<class T2>
    Group<const T2> getOf() const;
    template<class T2>
    Group<typename std::conditional<std::is_const<T>::value,
                                    const T2, T2>::type>
                    getOfOnly();
    template<class T2>
    Group<const T2> getOfOnly() const;

    virtual void set(const std::size_t i, T*);

    template<typename T2>
    Group<T> add(T2*);
    template<typename T2>
    Group<T> add(const std::vector<T2*>&);
    template<typename T2>
    Group<T> add(Group<T2>&);
    template<typename T2>
    Group<T> add(const Group<T2>&);
    virtual Group add(Group&);
    virtual Group add(Group&&);

    virtual void remove(const std::size_t&);
    virtual void remove(const std::vector<std::size_t>&);

private:
    std::vector<std::shared_ptr<T>> element_;

    template<typename T2>
    std::vector<std::size_t> getElemsOf_       () const;
    template<typename T2>
    std::vector<std::size_t> getElemsNotOf_    () const;
    template<typename T2>
    std::vector<std::size_t> getElemsOfOnly_   () const;
    template<typename T2>
    std::vector<std::size_t> getElemsNotOfOnly_() const;

    template<typename T2>
    std::shared_ptr<T> getSharedPtr(T2*) const;
};

} /* namespace Group */
} /* namespace SEMBA */

#include "Group.hpp"

