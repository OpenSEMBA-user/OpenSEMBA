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

#ifndef SEMBA_MATH_TYPES_H_
#define SEMBA_MATH_TYPES_H_

#include <cstdint>
#include <cstddef>

namespace SEMBA {
namespace Math {

#if   defined CompileWithInt1
typedef   int8_t  Int;
typedef  uint8_t UInt;
#elif defined CompileWithInt2
typedef  int16_t  Int;
typedef uint16_t UInt;
#elif defined CompileWithInt4
typedef  int32_t  Int;
typedef uint32_t UInt;
#elif defined CompileWithInt8
typedef  int64_t  Int;
typedef uint64_t UInt;
#else
typedef          int  Int;
typedef unsigned int UInt;
#endif

#if   defined CompileWithReal4
typedef float       Real;
#elif defined CompileWithReal8
typedef double      Real;
#elif defined CompileWithReal16
typedef long double Real;
#else
typedef double      Real;
#endif

typedef std::size_t Size;

} /* namespace Math */
} /* namespace SEMBA */

#endif /* SEMBA_MATH_TYPES_H_ */
