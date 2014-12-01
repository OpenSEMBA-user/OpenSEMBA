#include <stdlib.h>
#include <string.h>

#include "arrayset.h"

// read an arrayset_t floating type
arrayset_t read_arrayset(hid_t file_id, const char* path)
{
    arrayset_t ars;
    int nbdims, i;
    char *params;
    char *dims;
    char *dim;
    params = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    dims  = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    dim = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(params, "");
    strcat(params, path);
    strcat(params, "/");
    strcat(params, P_DATA);

    ars.single.comment = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    ars.single.label = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    ars.single.physical_nature = (char *) malloc(ELEMENT_NAME_LENGTH
            * sizeof(char));
    ars.single.unit = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    strcpy(ars.single.comment, "");
    strcpy(ars.single.label, "");
    strcpy(ars.single.physical_nature, "");
    strcpy(ars.single.unit, "");

    ars.single = read_single(file_id, params);

    ars.data = read_dataset(file_id, params);

    strcpy(dims, "");
    strcat(dims, path);
    strcat(dims, "/");
    strcat(dims, P_DIMS);

    nbdims = read_number_of_children(file_id, (const char*) dims);

    ars.dims = (vector_t *) malloc(nbdims * sizeof(vector_t));
    ars.nbdims = nbdims;
    for (i = 0; i < nbdims; i++)
    {
        char *buf;
        buf = (char *) malloc(2* sizeof(char));
        //= strdup("");
        sprintf(buf, "%i", i + 1);
        strcpy(dim, "");
        strcat(dim, path);
        strcat(dim, "/");
        strcat(dim, P_DIM);
        strcat(dim, buf);
        ars.dims[i] = read_vector(file_id, dim);
	free(buf);
    }
    free(dim);
    free(dims);
    free(params);
    return ars;
}

// clear an arrayset_t
arrayset_t clear_content_ars(arrayset_t ars)
{
    int i;
    ars.data = clear_content_dataset(ars.data);
    if (ars.nbdims != 0)
    {
	for (i=0;i<=ars.nbdims;i++)
          vector_clear_content(ars.dims[i]);
        ars.nbdims = 0;
    }
}

// read an arrayset2_t floating type
arrayset2_t read_arrayset2(hid_t loc_id, const char* path)
{
    arrayset2_t ars2;
    ars2.single = read_single(loc_id, path);
    char datapath[ABSOLUTE_PATH_NAME_LENGTH];
    char dimpath[ABSOLUTE_PATH_NAME_LENGTH];

    strcpy(datapath, "");
    strcat(datapath, path);
    strcat(datapath, "/");
    strcat(datapath, P_DATA);
    ars2.data = read_vector(loc_id, datapath);

    strcpy(dimpath, "");
    strcat(dimpath, path);
    strcat(dimpath, "/");
    strcat(dimpath, P_DIM1);
    ars2.dim1 = read_vector(loc_id, dimpath);
    return ars2;
}

// clear an arrayset2_t
arrayset2_t clear_content_ars2(arrayset2_t ars2)
{
    vector_clear_content(ars2.data);
    vector_clear_content(ars2.dim1);
    return ars2;
}

// return a string representation of arrayset2_t
char* arrayset_to_string(arrayset2_t ars2)
{
    char* tostring2 = single_to_string(ars2.single);
    return tostring2;
}
