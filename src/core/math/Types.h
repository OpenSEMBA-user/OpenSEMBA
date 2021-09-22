

#pragma once

#ifdef _MSC_VER
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#endif

#include <cstdint>
#include <cstddef>
#include <cstdlib>

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

} /* namespace Math */
} /* namespace SEMBA */

