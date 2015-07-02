/*
 * Sequence.h
 *
 *  Created on: 22/6/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_SEQUENCE_H_
#define COMMON_BASE_SEQUENCE_SEQUENCE_H_

#include "Types.h"

#include "Reference.h"

#include "SequenceIterator.h"
#include "SequenceConstIterator.h"

template <typename T>
class Sequence {
public:
    typedef SequenceIterator     <T>      Iterator;
    typedef SequenceConstIterator<T> ConstIterator;

    Sequence();
    virtual ~Sequence();

    Iterator      begin()       { return Iterator     (*this, 0); }
    ConstIterator begin() const { return ConstIterator(*this, 0); }

    Iterator      end()       { return Iterator     (*this, size()); }
    ConstIterator end() const { return ConstIterator(*this, size()); }

    virtual UInt size () const = 0;
    virtual bool empty() const;

    virtual typename      Reference<T>::type operator()(UInt pos);
    virtual typename ConstReference<T>::type operator()(UInt pos) const;

    virtual typename      Reference<T>::type get(UInt pos) = 0;
    virtual typename ConstReference<T>::type get(UInt pos) const = 0;
};

#include "Sequence.hpp"
#include "SequenceIterator.hpp"
#include "SequenceConstIterator.hpp"

#endif /* COMMON_BASE_SEQUENCE_SEQUENCE_H_ */
