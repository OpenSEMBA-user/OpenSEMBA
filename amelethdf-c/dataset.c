#include <stdlib.h>
#include <string.h>

#include "dataset.h"

// read a dataset_t floating type

dataset_t read_dataset(hid_t loc_id, const char* path)
{
    dataset_t ds;
    hsize_t * dims;
    int length, i;
    int rank;
    H5T_class_t *type_class;
    herr_t status;
    hid_t dataspace;

    status = H5LTget_dataset_ndims(loc_id, path, &rank);
    if (status < 0)
        printf("can't read rank for %s\n", path);
    dims = (hsize_t *) malloc(rank * sizeof(hsize_t));
    type_class = (H5T_class_t *) malloc(sizeof(H5T_class_t));
    status = H5LTget_dataset_info(loc_id, path, dims, type_class, NULL);
    if (status < 0)
        printf("can't read dims for %s\n", path);
    length = dims[0];
    ds.dims = (hsize_t *) malloc(rank * sizeof(hsize_t));
    for (i = 0; i < rank; i++)
        ds.dims[rank - i - 1] = dims[i];
    for (i = 1; i < rank; i++)
        length = length * dims[i];
    ds.nbdims = rank;
    ds.ivalue = NULL;
    ds.rvalue = NULL;
    ds.cvalue = NULL;
    if (type_class[0] == H5T_INTEGER)
    {
        ds.ivalue = (int *) malloc(length * sizeof(int));
        status = H5LTread_dataset_int(loc_id, path, ds.ivalue);
        if (status < 0)
        {
            printf("Can read integer value for %s\n", path);
        }
    }
    else if (type_class[0] == H5T_FLOAT)
    {
        ds.rvalue = (float *) malloc(length * sizeof(float));
        status = H5LTread_dataset_float(loc_id, path, ds.rvalue);
        if (status < 0)
        {
            printf("Can read float value for %s\n", path);
        }
    }
    else if (type_class[0] == H5T_COMPOUND)
    {
        ds.cvalue = (float_complex *)malloc(length*sizeof(float_complex));
        ds.cvalue = read_complex_dataset(loc_id, path);
    }
    ds.single = read_single(loc_id, path);
    free(dims);
    free(type_class);
    return ds;
}

//clear dataset
dataset_t clear_content_dataset(dataset_t dataset)
{
    dataset_t ds;
    ds.single = clear_content_single(ds.single);
    if (ds.dims != NULL)
        free(ds.dims);
    if (ds.ivalue != NULL)
        free(ds.ivalue);
    if (ds.rvalue != NULL)
        free(ds.rvalue);
    if (ds.cvalue != NULL)
        free(ds.cvalue);
    return ds;
}

// Given a shape and indices returns the index the element
// in memory
// For a (/6, 8, 4, 9/) and (/3, 5, 3, 7/) returns 1275
int get_index_dataset(hsize_t* indices, hsize_t* ishape)
{
    int res, i, j, prod;
    res = indices[0];
    int size_indices = (int) sizeof(indices) / sizeof(int);
    for (i = 1; i < size_indices; i++)
    {
        prod = ishape[0] * (indices[i] - 1);
        for (j = 1; j < (i - 1); j++)
            prod = prod * ishape[j] * (indices[i] - 1);
        res = res + prod;
    }
    return res;
}

herr_t get_check_dataset(dataset_t ds, hsize_t* indices, int datatype)
{
    herr_t status = 0;
    if (datatype == P_INTEGER)
    {
        if (ds.ivalue == NULL)
        {
            status = -1;
            printf("DataSet has not integer values\n");
        }
    }
    else if (datatype == P_REAL)
    {
        if (ds.rvalue == NULL)
        {
            status = -1;
            printf("DataSet has not float values\n");
        }
    }
    else if (datatype == P_COMPLEX)
    {
        if (ds.cvalue == NULL)
        {
            status = -1;
            printf("DataSet has not complex float values\n");
        }
    }

    if (sizeof(indices) != sizeof(ds.dims))
    {
        status = -1;
        printf("Wrong indices length\n");
    }
    return status;
}

// Given an integer dataset return the value at indices
int get_integer_dataset(dataset_t ds, hsize_t *indices)
{
    int rdata;
    herr_t status;
    status = get_check_dataset(ds, indices, P_INTEGER);
    rdata = ds.ivalue[get_index_dataset(indices, ds.dims)];
    return rdata;
}

// Given a float dataset return the value at indices
float get_float_dataset(dataset_t ds, hsize_t *indices)
{
    float rdata;
    herr_t status;
    status = get_check_dataset(ds, indices, P_REAL);
    rdata = ds.rvalue[get_index_dataset(indices, ds.dims)];
    return rdata;
}

// Given a complex float dataset return the value at indices
float_complex get_complexfloat_dataset(dataset_t ds, hsize_t *indices)
{
    float_complex rdata;
    herr_t status;
    status = get_check_dataset(ds, indices, P_COMPLEX);
    rdata = ds.cvalue[get_index_dataset(indices, ds.dims)];
    return rdata;
}

//return a string representation of dataset_t
char * dataset_to_string(dataset_t ds)
{
    char *s;
    s = (char *) malloc(200 * sizeof(char));
    strcpy(s, "");
    strcat(s, single_to_string(ds.single));
    return s;
}
