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

   const T* operator()(const UInt i) const;
   T* set(const UInt i) const;
   T operator[](const UInt i) const;
   CartesianVector<T,D>
   getCVec(const UInt i) const;
   UInt
   getDOFs() const;
   UInt size() const;

   void
   set(const UInt i, const CartesianVector<T,D>& vec);
   void
   set(const UInt i, const T& num);
   void
   setSize(const UInt siz);
   void
   setToZero();
   void
   setToOne();
   void
   setToRandom(const Real min, const Real max);

   void
   prod(const UInt init, const UInt end, const T param);
   void
   prod_omp(const UInt init, const UInt end, const T param);
   void
   addProd(
         const UInt init, const UInt end,
         const Field<T,D>& field, const T param);
   void
   addProd_omp(
         const UInt init, const UInt end,
         const Field<T,D>& field, const T param);
   void
   copy(const UInt init, const UInt end, const Field<T,D>& param);

   void
   swap(Field<T,D>& param, const UInt first, const UInt last);
private:
   T* val_;
   UInt size_;
};

#include "Field.hpp"

typedef Field<Real,3> FieldR3;

#endif /* FIELD_H_ */
