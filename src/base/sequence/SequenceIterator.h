/*
 * SequenceIterator.h
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_SEQUENCEITERATOR_H_
#define COMMON_BASE_SEQUENCE_SEQUENCEITERATOR_H_

#include <cstdlib>

#include "../../Types.h"
#include "../error/Error.h"

template<typename T>
class Sequence;

template<typename T>
class SequenceIterator {
public:
    SequenceIterator();
    SequenceIterator(Sequence<T>& seq, UInt pos);
    SequenceIterator(const SequenceIterator<T>& rhs);
    virtual ~SequenceIterator();

    SequenceIterator<T>& operator=(const SequenceIterator<T>& rhs);

    bool operator!=(const SequenceIterator<T>& rhs);

    Reference<T> operator* ();
    Reference<T> operator->();
    ConstReference<T> operator* () const;
    ConstReference<T> operator->() const;

    SequenceIterator<T>& operator++();

private:
    Sequence<T>* seq_;
    UInt pos_;
};

#endif /* COMMON_BASE_SEQUENCE_SEQUENCEITERATOR_H_ */
