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
 * Field.h
 *
 *  Created on: Sep 23, 2014
 *      Author: luis
 */

#ifndef FIELD_H_
#define FIELD_H_

#include <cstdlib>

using namespace std;

#include "CartesianVector.h"

template <class T, Int D>
class Field {
public:
   Field();
   Field(UInt size);
   virtual ~Field();

   T* operator()(const UInt i);
   const T* operator()(const UInt i) const;
   T* set(const UInt i) const;
   T operator[](const UInt i) const;
   CartesianVector<T,D> getCVec(const UInt i) const;
   UInt getDOFs() const;
   UInt size() const;

   void set(const UInt i, const CartesianVector<T,D>& vec);
   void set(const UInt i, const T& num);
   void setSize(const UInt siz);
   void setToZero();
   void setToOne();
   void setToRandom(const Real min, const Real max);

   void prod(const UInt init, const UInt end, const T param);
   void prod_omp(const UInt init, const UInt end, const T param);
   void addProd(
         const UInt init, const UInt end,
         const Field<T,D>& field, const T param);
   void addProd_omp(
         const UInt init, const UInt end,
         const Field<T,D>& field, const T param);
   void copy(const UInt init, const UInt end, const Field<T,D>& param);

   void swap(Field<T,D>& param, const UInt first, const UInt last);
private:
   T* val_;
   UInt size_;
};

#include "Field.hpp"

typedef Field<Real,1> FieldR1;
typedef Field<Real,3> FieldR3;
typedef Field<complex<Real>,3> FieldC3;

#endif /* FIELD_H_ */
