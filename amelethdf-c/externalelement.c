#include <string.h>
#include <stdlib.h>

#include "externalelement.h"
#include "set.h"
#include "stringdataset.h"

external_element_t read_external_elements(hid_t loc_id, char* path)
{
    int i, j;
    int *lmn;
    external_element_t ex_elt;

    lmn = get_dataset_lmn(loc_id, path);
    char *** buf;
    //alocate memory
    buf = (char ***) malloc(lmn[2] * sizeof(char **));
    for (i = 0; i < lmn[2]; i++)
    {
        buf[i] = (char **) malloc(lmn[2] * lmn[1] * sizeof(char *));
        buf[i][0] = (char *) malloc(lmn[2] * lmn[1] * lmn[0] * sizeof(char));
        for (j = 1; j < lmn[1]; j++)
            buf[i][j] = buf[i][0] + j * lmn[0];
    }

    buf = read_string_dataset1(loc_id, path, lmn[0], lmn[1], lmn[2]);
    ex_elt.external_name = (char **) malloc(lmn[2] * sizeof(char*));
    ex_elt.file_name = (char **) malloc(lmn[2] * sizeof(char*));
    ex_elt.internal_name = (char **) malloc(lmn[2] * sizeof(char*));

    ex_elt.external_name[0] = (char *) malloc(lmn[2] * lmn[0] * sizeof(char));
    ex_elt.file_name[0] = (char *) malloc(lmn[2] * lmn[0] * sizeof(char));
    ex_elt.internal_name[0] = (char *) malloc(lmn[2] * lmn[0] * sizeof(char));

    for (j = 1; j < lmn[1]; j++)
    {
        ex_elt.external_name[j] = ex_elt.external_name[0] + j * lmn[0];
        ex_elt.file_name[j] = ex_elt.file_name[0] + j * lmn[0];
        ex_elt.internal_name[j] = ex_elt.internal_name[0] + j * lmn[0];
    }
    ex_elt.nb_ext_el = lmn[2];
    for (i = 0; i < lmn[2]; i++)
    {
        strcpy(ex_elt.external_name[i], buf[i][0]);
        strcpy(ex_elt.file_name[i], buf[i][1]);
        strcpy(ex_elt.internal_name[i], buf[i][2]);
    }
    return ex_elt;
}

// Find out the path in external_elements and return the index
// Set the global current_index to the found value

int get_index_ext_elt(external_element_t ext_elt, char* path)
{
    int i;
    int index = -1;
    for (i = 0; i < ext_elt.nb_ext_el; i++)
    {
        if (strcmp(path, ext_elt.internal_name[i]) == 0)
            index = i;
    }
    return index;
}

char * get_file_ext_elt(external_element_t ext_elt, char* path)
{
    char * filename;
    int id;

    filename = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(filename, "");
    id = get_index_ext_elt(ext_elt, path);
    if (id > -1)
        strcat(filename, ext_elt.file_name[id]);
    return filename;
}

hid_t get_file_id_ext_elt(external_element_t ext_elt, char * path)
{
    hid_t file_id;
    int id;

    file_id = -1;
    id = get_index_ext_elt(ext_elt, path);
    if (id > -1)
        file_id = ext_elt.file_id[id];
    return file_id;
}

char * get_external_name(external_element_t ext_elt, char* path)
{
    char* external_name;
    int id;

    external_name = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(external_name, "");
    id = get_index_ext_elt(ext_elt, path);
    if (id > -1)
        strcat(external_name, ext_elt.external_name[id]);
    return external_name;
}

// Open all external files and store the id
external_element_t open_external_files(external_element_t ext_elt)
{
    hid_t file_id;
    hid_t *buf_id;
    set_t buf;
    herr_t status;
    int i, j;
    char * name;
    name = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    ext_elt.file_id = (hid_t*) malloc(ext_elt.nb_ext_el * sizeof(hid_t));
    buf_id = (hid_t*) malloc(ext_elt.nb_ext_el * sizeof(hid_t));
    buf.array = (char **) malloc(ext_elt.nb_ext_el * sizeof(char*));
    buf.array[0] = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
            * ext_elt.nb_ext_el * sizeof(char));
    for (i = 1; i < ext_elt.nb_ext_el; i++)
        buf.array[i] = buf.array[0] + i * ABSOLUTE_PATH_NAME_LENGTH;
    for (i = 0; i < ext_elt.nb_ext_el; i++)
    {
        name = strdup(ext_elt.file_name[i]);
        if (!in(buf, name))
        {
            file_id = H5Fopen(name, H5F_ACC_RDONLY, H5P_DEFAULT);
            buf = add(buf, name);
            ext_elt.file_id[i] = file_id;
            buf_id[buf.number_of_element] = file_id;
        }
        else
        {
            j = str_index(buf.array, name, buf.number_of_element);
            ext_elt.file_id[i] = buf_id[j];
        }
    }
    return ext_elt;
}

// Close all external files

herr_t close_external_files(external_element_t ext_elt)
{
    char* name;
    hid_t file_id;
    int i;
    set_t buf;
    herr_t status, status2;
    status = 0;
    name = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(name, "");
    buf.array = (char **) malloc(ext_elt.nb_ext_el * sizeof(char *));
    buf.array[0] = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
            * ext_elt.nb_ext_el * sizeof(char));
    for (i = 1; i < ext_elt.nb_ext_el; i++)
        buf.array[i] = buf.array[0] + i * ABSOLUTE_PATH_NAME_LENGTH;
    for (i = 0; ext_elt.nb_ext_el; i++)
    {
        strcat(name, ext_elt.file_name[i]);
        if (!in(buf, name))
        {
            file_id = ext_elt.file_id[i];
            status2 = H5Fclose(file_id);
            if (status2 < 0)
            {
                printf("Can't close file :%s\n", name);
                status = status2;
            }
            buf = add(buf, name);
        }
    }

    free(name);
    return status2;
}
