#ifndef FLOATINGTYPE_H
#define FLOATINGTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arrayset.h"
#define A_FLOATING_TYPE "floatingType"
#define V_SINGLE_REAL "singleReal"
#define V_SINGLE_COMPLEX "singleComplex"
#define V_DATA_SET "dataSet"
#define V_ARRAY_SET "arraySet"
#define V_VECTOR "vector"

#define E_SINGLE_REAL 1
#define E_SINGLE_COMPLEX 2
#define E_VECTOR 3
#define E_DATA_SET 4
#define E_ARRAY_SET 5

typedef struct
{
        int floatingtype;
        singlereal_t singlereal;
        singlecomplex_t singlecomplex;
        vector_t vector;
        dataset_t dataset;
        arrayset_t arrayset;
} floatingtype_t;

floatingtype_t read_floatingtype(hid_t loc_id, const char *path);
int get_type_ft(hid_t loc_id, const char* path);
floatingtype_t clear_content_ft(floatingtype_t ft);
int issinglereal(floatingtype_t ft);
int issinglecomplex(floatingtype_t ft);
int isvector(floatingtype_t ft);
int isdataset(floatingtype_t ft);
int isarrayset(floatingtype_t ft);

#ifdef __cplusplus
}
#endif

#endif // FLOATINGTYPE_H
