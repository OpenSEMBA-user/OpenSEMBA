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
#include <sstream>
#include <string>

namespace SEMBA {
namespace Group {
namespace Error {
namespace Id {

template <typename Identification>
class Base : public std::exception {
public:
    Base(const Identification& id) : id_(id) {}
    virtual ~Base() throw() {}

    Identification getId() const { return id_; }
private:
    Identification id_;
};

template <typename Identification>
class NotExists : public Base<Identification> {
public:
    NotExists(const Identification& id)
    :   Base<Identification>(id) {
        str_ = std::string("Group: Element with id ") +
               this->getId().toStr() +
               std::string(" not exists");
    }
    virtual ~NotExists() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

template <typename Identification>
class Zero : public Base<Identification> {
public:
    Zero() : Base<Identification>(Identification(0)) {}
    virtual ~Zero() throw() {}

    const char* what() const throw() {
        return "Group: Element with id 0";
    }
};

template <typename Identification>
class Duplicated : public Base<Identification> {
public:
    Duplicated(const Identification& id)
    :   Base<Identification>(id) {
        str_ = std::string("Group: Duplicated Id ") +
               this->getId().toStr();
    }
    virtual ~Duplicated() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Id */
} /* namespace Error */
} /* namespace Group */
} /* namespace SEMBA */

