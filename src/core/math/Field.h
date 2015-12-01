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

#ifndef FIELD_H_
#define FIELD_H_

#include <cstdlib>

#include "math/vector/Cartesian.h"

namespace SEMBA {
namespace Math {

template <class T, Size D>
class Field {
public:
    Field();
    Field(Size size);
    virtual ~Field();

    T*       operator()(const Size i);
    const T* operator()(const Size i) const;

    T operator[](const Size i) const;

    Vector::Cartesian<T,D> getCVec(const Size i) const;

    Size getDOFs() const;
    Size size() const;

    T* set(const Size i) const;
    void set(const Size i, const T& num);
    void set(const Size i, const Vector::Cartesian<T,D>& vec);

    void setAll(const T& num);
    void setSize(const Size siz);
    void setToRandom(const Real min, const Real max);

    void prod(const Size init, const Size end, const T param);
    void prod_omp(const Size init, const Size end, const T param);
    void addProd(const Size init, const Size end,
                 const Field<T,D>& field, const T param);
    void addProd_omp(const Size init, const Size end,
                     const Field<T,D>& field, const T param);
    void copy(const Size init, const Size end, const Field<T,D>& param);

    void swap(Field<T,D>& param, const Size first, const Size last);
private:
    T* val_;
    Size size_;
};

} /* namespace Math */
} /* namespace SEMBA */

#include "Field.hpp"

namespace SEMBA {
namespace Math {

typedef Field<Real,1> FieldR1;
typedef Field<Real,3> FieldR3;
typedef Field<std::complex<Real>,3> FieldC3;

} /* namespace Math */
} /* namespace SEMBA */

#endif /* FIELD_H_ */
