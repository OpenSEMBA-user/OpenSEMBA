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
