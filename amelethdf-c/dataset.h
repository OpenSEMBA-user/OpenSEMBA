#ifndef DATASET_H
#define DATASET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector.h"
#define V_DATASET "dataset"
#define P_INTEGER 1
#define P_REAL    2
#define P_COMPLEX 3

dataset_t read_dataset(hid_t loc_id, const char* path);
dataset_t clear_content_dataset(dataset_t dataset);
int get_index_dataset(hsize_t * indices, hsize_t* ishape);
herr_t get_check_dataset(dataset_t ds, hsize_t *indices, int datatype);
int get_integer_dataset(dataset_t ds, hsize_t *indices);
float get_float_dataset(dataset_t ds, hsize_t *indices);
float_complex get_complexfloat_dataset(dataset_t ds, hsize_t *indices);
char * dataset_to_string(dataset_t ds);

#ifdef __cplusplus
}
#endif

#endif // DATASET_H
