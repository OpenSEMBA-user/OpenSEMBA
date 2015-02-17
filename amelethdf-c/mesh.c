#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "stringdataset.h"

int meshtype(hid_t loc_id, const char * path)
{
    herr_t status;
    hid_t attr_id, attr_type, space, memtype, mesh_id;
    size_t sdim;
    hsize_t dims[1] = { 1 };
    int ndims, i, ret_val = -1;
    char **rdata;

    mesh_id = H5Gopen1(loc_id, path);

    ret_val = 0;
    attr_id = H5Aopen(mesh_id, "type", H5P_DEFAULT);

    if (attr_id >= 0)
    {
        attr_type = H5Aget_type(attr_id);
        sdim = H5Tget_size(attr_type);
        sdim++; // Make room for null terminator
        space = H5Aget_space(attr_id);
        ndims = H5Sget_simple_extent_dims(space, dims, NULL);
        rdata = (char **) malloc(dims[0] * sizeof(char *));
        rdata[0] = (char *) malloc(dims[0] * sdim * sizeof(char));

        // Set the rest of the pointers to rows to the correct addresses.
        for (i = 1; i < dims[0]; i++)
            rdata[i] = rdata[0] + i * sdim;
        memtype = H5Tcopy(H5T_C_S1);
        status = H5Tset_size(memtype, sdim);
        status = H5Aread(attr_id, memtype, rdata[0]);
        // Check the value of FORMAT attribute.
        if (strcmp(rdata[0], "unstructured") == 0)
            ret_val = UNSTRUCTURED;
        else if (strcmp(rdata[0], "structured") == 0)
            ret_val = STRUCTURED;
        H5Aclose(attr_id);
        free(rdata[0]);
        free(rdata);
    }
    H5Gclose(mesh_id);

    return ret_val;

}

groupgroup_t readGroupGroup(hid_t grpgrp_id, const char* name)
{
    hid_t space, filetype, memtype;
    int ndims, i;
    hsize_t dims[1];
    groupgroup_t rdata;
    herr_t status;
    size_t sdim;

    rdata.name = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    strcpy(rdata.name, name);
    filetype = H5Dget_type(grpgrp_id);
    sdim = H5Tget_size(filetype);
    sdim++; // make room for null terminator

    space = H5Dget_space(grpgrp_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);

    // allocate array of pointers to rows
    rdata.groupGroupnames = (char **) malloc(dims[0] * sizeof(char *));
    // allocate space for integer data
    rdata.groupGroupnames[0] = (char *) malloc(dims[0] * sdim * sizeof(char));
    rdata.nbeltgroupGroup = dims[0];
    for (i = 1; i < dims[0]; i++)
        rdata.groupGroupnames[i] = rdata.groupGroupnames[0] + i * sdim;

    // create the memory datatype
    memtype = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(memtype, sdim);

    // read the data
    status = H5Dread(grpgrp_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.groupGroupnames[0]);
    for (i=0; i< dims[0]; i++)
        strcpy(rdata.groupGroupnames[i],remove_space(rdata.groupGroupnames[i]));

    return rdata;
}

dimptsinelts_t readNbSemPtInElt(hid_t file_id, const char *path)
{
    herr_t status;
    dimptsinelts_t data;
    hid_t table_id;
    hsize_t nfields_out;
    hsize_t nrecords_out;
    int i;
    children_t children;
    char *bufpath;
    
    children = read_children_name(file_id, path);
    data.nbptinelt = (hsize_t *) malloc(children.nbchild * sizeof(hsize_t));
    data.name = (char **) malloc(children.nbchild * sizeof(char *));
    data.nb = 0;
    for(i = 0; i < children.nbchild; i++)
    {
        data.name[i] = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
        bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        strcpy(bufpath, path);
        strcat(bufpath, "/");
        strcat(bufpath, children.childnames[i]);    
        char * type;
        type = malloc( ELEMENT_NAME_LENGTH * sizeof(char));
        type = read_string_attribute(file_id, bufpath, A_TYPE);
        
        if(strcmp(type,"pointInElement")==0){
          table_id = H5TBget_table_info(file_id, bufpath, &nfields_out,
            &nrecords_out);
          data.nbptinelt[data.nb]=nrecords_out;
          strcpy(data.name[data.nb],children.childnames[i]);
          data.nb++;
        }
        free(bufpath);
    }    
    return data;
}
