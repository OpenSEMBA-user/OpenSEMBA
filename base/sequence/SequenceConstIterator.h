/*
 * SequenceConstIterator.h
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_SEQUENCECONSTITERATOR_H_
#define COMMON_BASE_SEQUENCE_SEQUENCECONSTITERATOR_H_

#include <cstdlib>

#include "SequenceIterator.h"

template<typename T>
class SequenceConstIterator {
public:
    SequenceConstIterator();
    SequenceConstIterator(const Sequence<T>& seq, UInt pos);
    SequenceConstIterator(const SequenceIterator<T>& rhs);
    SequenceConstIterator(const SequenceConstIterator<T>& rhs);
    virtual ~SequenceConstIterator();

    SequenceConstIterator<T>& operator=(const SequenceIterator<T>& rhs);
    SequenceConstIterator<T>& operator=(const SequenceConstIterator<T>& rhs);

    bool operator!=(const SequenceConstIterator<T>& rhs);

    ConstReference<T> operator* () const;
    ConstReference<T> operator->() const;

    SequenceConstIterator<T>& operator++();

private:
    const Sequence<T>* seq_;
    UInt pos_;
};

#endif /* COMMON_BASE_SEQUENCE_SEQUENCECONSTITERATOR_H_ */
