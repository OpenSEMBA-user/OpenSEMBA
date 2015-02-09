#ifndef SIMPLETYPE_H
#define SIMPLETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "complextype.h"
#define A_LABEL  "label"
#define A_PHYSICAL_NATURE  "physicalNature"
#define A_UNIT  "unit"
#define A_COMMENT  "comment"
#define A_VALUE  "value"

typedef struct
{
        char *label;
        char *physical_nature;
        char *unit;
        char *comment;
} single_t;

typedef struct
{
        single_t single;
        float value;
} singlereal_t;

typedef struct
{
        single_t single;
        float_complex value;
} singlecomplex_t;

typedef struct
{
        single_t single;
        hsize_t *dims;
        int nbdims;
        int *ivalue;
        float *rvalue;
        float_complex *cvalue;
} dataset_t;

typedef struct
{
        float first;
        float last;
        int number_of_values;
} linearlistofreal1_t;

typedef struct
{
        float first;
        float step;
        int number_of_values;
} linearlistofreal2_t;

typedef struct
{
        float first;
        float last;
        int number_of_values;
} logarithmlistofreal_t;

typedef struct
{
        float first;
        int number_of_decades;
        int number_of_values_per_decade;
} perdecadelistofreal_t;

typedef struct
{
        int first;
        int step;
        int number_of_values;
} linearlistofinteger2_t;

single_t read_single(hid_t loc_id, const char *path);
single_t clear_content_single(single_t single);
singlereal_t clear_content_singlereal(singlereal_t singlereal);
singlecomplex_t clear_content_singlecomplex(singlecomplex_t singlcomplex);
singlereal_t read_singlereal(hid_t loc_id, const char *path);
singlecomplex_t read_singlecomplex(hid_t loc_id, const char *path);

char* single_to_string(single_t single);
char* singlereal_to_string(singlereal_t singlereal);
char* singlecomplex_to_string(singlecomplex_t singlecomplex);

herr_t make_single(hid_t loc_id, const char* path, single_t single);

#ifdef __cplusplus
}
#endif

#endif // SIMPLETYPE_H
