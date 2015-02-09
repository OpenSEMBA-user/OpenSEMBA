#ifndef AMELETHDF_H
#define AMELETHDF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdf5.h>
#include <hdf5_hl.h>


#define ABSOLUTE_PATH_NAME_LENGTH 101
#define ELEMENT_NAME_LENGTH 51

#define TRUE 1
#define FALSE 0

typedef struct
{
    char ** childnames;
    int nbchild;
} children_t;


/*
 * Returns the number of children of a group at path
 */
int read_number_of_children(hid_t file_id, const char* path);

/*
 * Returns children of a group at path
 */
children_t read_children_name(hid_t loc_id, const char* path);

/*
 * Reads a string attribute at path
 */
char * read_string_attribute(hid_t loc_id, const char* path, char* attr);

/*
 * Reads a float attribute at path
 */
float read_float_attribute(hid_t loc_id, const char* path, char* attr);

/*
 * Reads a float attribute at path
 */
int read_int_attribute(hid_t loc_id, const char* path, char* attr);

#ifdef __cplusplus
}
#endif

#endif // AMELETHDF_H

