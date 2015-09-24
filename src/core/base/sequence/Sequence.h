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
