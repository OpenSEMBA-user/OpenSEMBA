#include <string.h>
#include <stdlib.h>
#include "amelethdf.h"

children_t read_children_name(hid_t loc_id, const char* path)
{
    H5G_info_t ginfo;
    children_t child;
    herr_t status;
    hsize_t i;
    ssize_t size;
    hid_t group_id;

    group_id = H5Gopen1(loc_id, path);

    status = H5Gget_info(group_id, &ginfo);

    // Get size of name, add 1 for null terminator
    child.nbchild = (int) ginfo.nlinks;
    child.childnames = (char **) malloc(child.nbchild * sizeof(char *));
    for (i = 0; i < child.nbchild; i++)
    {
        size = 1 + H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME,
                H5_ITER_INC, i, NULL, 0, H5P_DEFAULT);
        child.childnames[i] = (char *) malloc(size);
        size = H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i,
                child.childnames[i], (size_t) size, H5P_DEFAULT);
    }
    H5Gclose(group_id);
    return child;
}

char * read_string_attribute(hid_t loc_id, const char* path, char* attr)
{
    herr_t status;
    hsize_t dims[1] = { 1 };
    size_t sdim;
    char **rdata; /* Read buffer */
    char *attribute;
    int ndims, i;
    hid_t filetype, memtype, space, dset, attr_id;

    status = H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT);
    if (status < 0)
    {
        printf("%s doesn't exist for %s\n", attr, path);
        rdata = (char **) malloc(1 * sizeof(char*));
        rdata[0] = (char *) malloc(ELEMENT_NAME_LENGTH * sdim * sizeof(char));
        strcpy(rdata[0], "");
	attribute = (char *) malloc(ELEMENT_NAME_LENGTH * sdim * sizeof(char));
    }
    else
    {
        dset = H5Oopen(loc_id, path, H5P_DEFAULT);
        attr_id = H5Aopen(dset, attr, H5P_DEFAULT);
        filetype = H5Aget_type(attr_id);
        sdim = H5Tget_size(filetype);
        sdim++;
        space = H5Aget_space(attr_id);
        ndims = H5Sget_simple_extent_dims(space, dims, NULL);
        rdata = (char **) malloc(dims[0] * sizeof(char *));
        rdata[0] = (char *) malloc(dims[0] * sdim * sizeof(char));
	attribute = (char *) malloc(dims[0] * sdim * sizeof(char));
        for (i = 1; i < dims[0]; i++)
            rdata[i] = rdata[0] + i * sdim;
        memtype = H5Tcopy(H5T_C_S1);
        status = H5Tset_size(memtype, sdim);
        status = H5Aread(attr_id, memtype, rdata[0]);
        status = H5Oclose(dset);
        status = H5Sclose(space);
        status = H5Tclose(filetype);
        status = H5Tclose(memtype);
    }
    strcpy(attribute,rdata[0]);
    free(rdata[0]);
    free(rdata);
    return attribute;
}

float read_float_attribute(hid_t loc_id, const char* path, char* attr)
{
    herr_t status;
    float rdata;

    status = H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT);
    if (status < 0)
    {
        printf("%s doesn't exist for %s\n", attr, path);
    }
    else
    {
        H5LTget_attribute_float(loc_id, path, attr, &rdata);
    }
    return rdata;
}

int read_int_attribute(hid_t loc_id, const char* path, char* attr)
{
    herr_t status;
    int idata;

    status = H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT);
    if (status < 0)
    {
        printf("%s doesn't exist for %s\n", attr, path);
    }
    else
    {
        H5LTget_attribute_int(loc_id, path, attr, &idata);
    }
    return idata;
}

int read_number_of_children(hid_t file_id, const char* path)
{
    int nbchild;
    H5G_info_t ginfo;
    herr_t status;
    status = H5Gget_info_by_name(file_id, path, &ginfo, H5P_DEFAULT);
    // Get size of name, add 1 for null terminator.
    nbchild = (int) ginfo.nlinks;
    return nbchild;
}
