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

#ifndef SEMBA_CLASS_IDENTIFICATION_H_
#define SEMBA_CLASS_IDENTIFICATION_H_

#include <cstddef>
#include <iostream>
#include <string>

namespace SEMBA {
namespace Class {

template <typename T>
class Identification {
    template <typename I>
    friend std::istream& operator>>(std::istream&  input,
                                    Identification<I>& id);
    template <typename I>
    friend std::ostream& operator<<(std::ostream& output,
                                    const Identification<I>& id);
public:
    Identification();
    explicit Identification(const std::size_t id);
    Identification(const Identification& rhs);
    virtual ~Identification();

    Identification& operator =(const Identification& rhs);
    Identification& operator+=(const Identification& rhs);

    bool operator==(const Identification& rhs) const;
    bool operator!=(const Identification& rhs) const;
    bool operator< (const Identification& rhs) const;
    bool operator<=(const Identification& rhs) const;
    bool operator> (const Identification& rhs) const;
    bool operator>=(const Identification& rhs) const;

    Identification  operator+ (const Identification& rhs) const;
    Identification& operator++();
    Identification  operator++(int);

    std::size_t toInt() const;
    std::string toStr() const;

private:
    std::size_t id_;
};

} /* namespace Class */
} /* namespace SEMBA */

#include "Identification.hpp"

#endif /* SEMBA_CLASS_IDENTIFICATION_H_ */
