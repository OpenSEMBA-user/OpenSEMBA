#include <stdlib.h>
#include <string.h>

#include "vector.h"

vector_t read_vector(hid_t loc_id, const char* path)
{
    vector_t vec;
    hsize_t dims[1];
    H5T_class_t type_class;
    size_t type_size;
    herr_t status;
    int rank, i;

    dims[0]=1;
    // initializes vec
    vec.ivalue = NULL;
    vec.rvalue = NULL;
    vec.cvalue = NULL;
    vec.svalue = NULL;
    vec.single.comment = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    vec.single.label = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    vec.single.physical_nature = (char *) malloc(ELEMENT_NAME_LENGTH
            * sizeof(char));
    vec.single.unit = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    strcpy(vec.single.comment, "");
    strcpy(vec.single.label, "");
    strcpy(vec.single.physical_nature, "");
    strcpy(vec.single.unit, "");

    vec.single = read_single(loc_id, path);
    status = H5LTget_dataset_ndims(loc_id, path, &rank);
    //dims = (hsize_t *) malloc( rank * sizeof(hsize_t));
    status = H5LTget_dataset_info(loc_id, path, dims, &type_class, &type_size);
    vec.nbvalues = dims[0];
    if (type_class == H5T_INTEGER)
    {
        vec.ivalue = (int *) malloc(dims[0] * sizeof(int));
        status = H5LTread_dataset_int(loc_id, path, vec.ivalue);
    }
    else if (type_class == H5T_FLOAT)
    {
        vec.rvalue = (float *) malloc(dims[0] * sizeof(float));
        status = H5LTread_dataset_float(loc_id, path, vec.rvalue);
    }
    else if (type_class == H5T_COMPOUND)
    {
        vec.cvalue = (float_complex *)malloc(dims[0]*sizeof(float_complex));
        vec.cvalue = read_complex_dataset(loc_id, path);
    }
    else if (type_class == H5T_STRING)
    {
        vec.svalue = (char **) malloc(dims[0] * sizeof(char*));
        vec.svalue[0] = (char *) malloc(type_size * sizeof(char));
        if(vec.nbvalues>=2)
        {
            for (i = 1; i < dims[0]; i++)
                vec.svalue[i] = vec.svalue[0] + i * type_size;
        }
        vec.svalue = (char **) read_string_dataset2(loc_id, path, type_size,
                                                    dims[0]);
    }
    //free(dims);

    return vec;
}

vector_t vector_clear_content(vector_t vec)
{
    vec.single = clear_content_single(vec.single);
    if (vec.ivalue != NULL)
        free(vec.ivalue);
    if (vec.rvalue != NULL)
        free(vec.rvalue);
    if (vec.cvalue != NULL)
        free(vec.cvalue);

    return vec;
}

char * vector_to_string(vector_t vec)
{
    char* buf = "";
    char* s;
    int i;
    s = single_to_string(vec.single);
    if (vec.ivalue != NULL)
    {
        for (i = 0; i < vec.nbvalues; i++)
            sprintf(buf, " %i ", vec.ivalue[i]);
    }
    else if (vec.rvalue != NULL)
    {
        for (i = 0; i < vec.nbvalues; i++)
            sprintf(buf, " %f ", vec.rvalue[i]);
    }
    else if (vec.cvalue != NULL)
    {
        for (i = 0; i < vec.nbvalues; i++)
            sprintf(buf, " %f + i %f", crealf(vec.cvalue[i]), cimagf(
                    vec.cvalue[i]));
    }
    strcat(s, buf);
    return s;
}

herr_t write_vector(hid_t loc_id, const char* path, vector_t vec)
{
    herr_t status;
    hsize_t dims[1];

    dims[0] = vec.nbvalues;
    if (vec.ivalue != NULL)
    {
        status = H5LTmake_dataset_int(loc_id, path, 1, dims, vec.ivalue);
    }
    else if (vec.rvalue != NULL)
    {
        status = H5LTmake_dataset_float(loc_id, path, 1, dims, vec.rvalue);
    }
    else if (vec.cvalue != NULL)
    {
        status = write_complexe_1D_dataset(loc_id, path, vec.cvalue,
                vec.nbvalues);
    }
    if (status < 0)
    {
        printf("Can't make dataset");
    }
    return status;
}
