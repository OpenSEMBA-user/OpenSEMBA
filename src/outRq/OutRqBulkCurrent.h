/*
 * OutRqBulkCurrent.h
 *
 *  Created on: 20 de ago. de 2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_OUTRQ_OUTRQBULKCURRENT_H_
#define SRC_COMMON_OUTRQ_OUTRQBULKCURRENT_H_

#include "OutRq.h"

class OutRqBulkCurrent : public virtual OutRq<Elem> {
public:
    OutRqBulkCurrent(const Domain& domain,
                     const string& name,
                     const GroupElements<Elem>& box,
                     const CartesianAxis& dir,
                     const UInt& skip);
    OutRqBulkCurrent(const OutRqBulkCurrent& rhs);
    virtual ~OutRqBulkCurrent();

    DEFINE_CLONE(OutRqBulkCurrent);

    bool hasSameProperties(const OutRqBase& rhs) const;

    CartesianAxis getDir() const;
    UInt          getSkip() const;

private:
    CartesianAxis dir_;
    UInt skip_;
};

#endif /* SRC_COMMON_OUTRQ_OUTRQBULKCURRENT_H_ */
