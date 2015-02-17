#ifndef ARRAYSET_H
#define ARRAYSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dataset.h"
#define P_DATA  "data"
#define P_DIMS  "ds"
#define P_DIM "ds/dim"
#define P_DIM1 "ds/dim1"

typedef struct
{
        single_t single;
        vector_t data;
        vector_t dim1;
} arrayset2_t;

typedef struct
{
        single_t single;
        dataset_t data;
        vector_t *dims;
        int nbdims;
} arrayset_t;

arrayset_t read_arrayset(hid_t loc_id, const char* path);
arrayset2_t read_arrayset2(hid_t loc_id, const char* path);
arrayset_t clear_content_ars(arrayset_t ars);
arrayset2_t clear_content_ars2(arrayset2_t ars2);
char* arrayset_to_string(arrayset2_t ars2);

#ifdef __cplusplus
}
#endif

#endif // ARRAYSET_H
