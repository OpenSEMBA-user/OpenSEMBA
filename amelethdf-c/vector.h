#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "simpletype.h"
typedef struct
{
        single_t single;
        int* ivalue;
        float* rvalue;
        float_complex* cvalue;
        char ** svalue;
        int nbvalues;
} vector_t;

vector_t read_vector(hid_t loc_id, const char* path);
char * vector_to_string(vector_t vec);
herr_t write_vector(hid_t loc_id, const char* path, vector_t vec);
vector_t vector_clear_content(vector_t vec);

#ifdef __cplusplus
}
#endif

#endif // VECTOR_H
