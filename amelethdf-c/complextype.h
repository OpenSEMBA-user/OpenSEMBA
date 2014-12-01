#ifndef COMPLEXTYPE_H
#define COMPLEXTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#if __STDC_VERSION__ >= 199901L
//using a C99 compiler
#include <complex.h>
typedef float _Complex float_complex;
#else
typedef struct 
{
    float re, im;
} float_complex;
#endif

#if __STDC_VERSION__ >= 199901L
//creal, cimag already defined in complex.h

__inline float_complex make_complex_float(float real, float imag);
__inline float abs_complex(float_complex a);
#else
#define crealf(z) ((z).re)
#define cimagf(z) ((z).im)

extern const float_complex complex_i; //put in a translation unit somewhere
#define I complex_i
__inline float_complex make_complex_float(float real, float imag);
__inline float abs_complex(float_complex a);
#endif



hid_t create_type_id(hid_t real_or_double);
hid_t create_real_type_id();
hid_t create_double_type_id();
herr_t write_complex_type(hid_t loc_id);
herr_t create_complex_attribute(hid_t loc_id, const char* path,
        const char* name, float_complex value);
float_complex read_complex_attribute(hid_t loc_id, const char* path,
        const char* name);
float_complex *read_complex_dataset(hid_t loc_id, const char* path);
herr_t write_complexe_1D_dataset(hid_t loc_id, const char* path,
        float_complex* values, int nbvalues);

#ifdef __cplusplus
}
#endif

#endif // COMPLEXTYPE_H
