#ifndef EXTERNALELEMENT_H
#define EXTERNALELEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"
typedef struct
{
        char **internal_name;
        char **file_name;
        char **external_name;
        int nb_ext_el;
        hid_t *file_id;
} external_element_t;

external_element_t read_external_elements(hid_t loc_id, char* path);
int get_index_ext_elt(external_element_t ext_elt, char* path);
char * get_file_ext_elt(external_element_t ext_elt, char* path);
hid_t get_file_id_ext_elt(external_element_t ext_elt, char * path);
char * get_external_name(external_element_t ext_elt, char* path);
external_element_t open_external_files(external_element_t ext_elt);
herr_t close_external_files(external_element_t ext_elt);

#ifdef __cplusplus
}
#endif

#endif // EXTERNALELEMENT_H
