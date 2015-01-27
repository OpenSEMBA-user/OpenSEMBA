/*
 * Field.h
 *
 *  Created on: Sep 23, 2014
 *      Author: luis
 */

#ifndef FIELD_H_
#define FIELD_H_

#include "CartesianVector.h"
#include <stdlib.h>

template <class T, int D>
class Field {
public:
	Field();
	Field(uint size);
	virtual ~Field();

	const T* operator()(const uint i) const;
	T* set(const uint i) const;
	T operator[](const uint i) const;
	CartesianVector<T,D>
	getCVec(const uint i) const;
	uint
	 getDOFs() const;

	void
	 set(const uint i, const CartesianVector<T,D>& vec);
	void
	 set(const uint i, const T& num);
	void
	 setSize(const uint siz);
	void
	 setToZero();
	void
	 setToOne();
	void
	 setToRandom(const double min, const double max);

	void
	 prod(const uint init, const uint end, const T param);
	void
	 prod_omp(const uint init, const uint end, const T param);
	void
	 addProd(
	  const uint init, const uint end,
	  const Field<T,D>& field, const T param);
	void
	 addProd_omp(
	  const uint init, const uint end,
	  const Field<T,D>& field, const T param);
	void
	 copy(const uint init, const uint end, const Field<T,D>& param);

	void
	 swap(Field<T,D>& param, const uint first, const uint last);
private:
	T* val_;
	uint size_;
};

#include "Field.hpp"

typedef Field<double,3> FieldD3;

#endif /* FIELD_H_ */
