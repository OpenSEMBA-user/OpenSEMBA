#ifdef __cplusplus
extern "C" {
#endif


#include <hdf5.h>
#include <hdf5_hl.h>
#include <string.h>
#include "amelethdf.h"

char ** read_string_dataset2(hid_t file_id, const char * path, size_t length, hsize_t mn);
char *** read_string_dataset1(hid_t file_id, const char * path, int l, int m,
        int n);
int * get_dataset_lmn(hid_t file_id, char* path);
void get_dataset_dims(hid_t file_id, const char* path, hsize_t *m, hsize_t *n, size_t *l);
children_t read_string_vector(hid_t file_id, char* path);
int str_index(char ** arr, char *aelement, int nbel);
void write_string_dataset(hid_t file_id, char* path, int values_len, int rank,
        hsize_t* dims, char** values);
char *remove_space(char *name);

#ifdef __cplusplus
}
#endif
