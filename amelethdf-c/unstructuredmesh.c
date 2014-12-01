#include <stdlib.h>
#include <string.h>

#include "unstructuredmesh.h"

nodes_t readNodes(hid_t nodes_id)
{
    hid_t space;
    int ndims, i;

    hsize_t dims[2];
    nodes_t rdata;
    herr_t status;

    /*
     * Get dataspace and allocate memory for read buffer.  HDF5Nodes is a
     * two dimensional dataset so the dynamic allocation must be done
     * in steps.
     */
    space = H5Dget_space(nodes_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);

    /*
     * Allocate array of pointers to rows.
     */
    rdata.nodes = (float **) malloc(dims[0] * sizeof(float *));

    /*
     * Allocate space for floating point data.
     */
    rdata.nodes[0] = (float *) malloc(dims[0] * dims[1] * sizeof(float));

    /*
     * Set the rest of the pointers to rows to the correct addresses.
     */
    for (i = 1; i < dims[0]; i++)
        rdata.nodes[i] = rdata.nodes[0] + i * dims[1];
    rdata.nbnodes = dims[0];
    status = H5Dread(nodes_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.nodes[0]);
    return rdata;
}

elttypes_t readElementTypes(hid_t eltypes_id)
{
    hid_t space;
    int ndims, i;
    hsize_t dims[1];
    elttypes_t rdata;
    herr_t status;

    space = H5Dget_space(eltypes_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    rdata.elttypes = (int *) malloc(dims[0] * sizeof(int));
    rdata.nbelttypes = dims[0];
    status = H5Dread(eltypes_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.elttypes);
    return rdata;
}

eltnodes_t readElementNodes(hid_t elnodes_id)
{
    hid_t space;
    int ndims, i;
    hsize_t dims[1];
    eltnodes_t rdata;
    herr_t status;

    space = H5Dget_space(elnodes_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    rdata.eltnodes = (int *) malloc(dims[0] * sizeof(int *));
    rdata.nbeltnodes = dims[0];
    status = H5Dread(elnodes_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.eltnodes);
    return rdata;
}


usemptsinelt_t readUSemPtInElt(hid_t file_id, const char *table_name)
{
    herr_t status;
    hsize_t nfields_out;
    hsize_t nrecords_out;
    hid_t table_id;
    usemptsinelt_t rdata;

    size_t dst_size = sizeof(usemptinelt_t);
    size_t dst_offset[5] =
    { HOFFSET( usemptinelt_t, index ), HOFFSET( usemptinelt_t, v1 ), 
      HOFFSET( usemptinelt_t, v2 ), HOFFSET( usemptinelt_t, v3 ) };

    size_t dst_sizes[5] =
    { sizeof(rdata.usemptinelt[0].index), sizeof(rdata.usemptinelt[0].v1),
      sizeof(rdata.usemptinelt[0].v2), sizeof(rdata.usemptinelt[0].v3) };

    table_id = H5TBget_table_info(file_id, table_name, &nfields_out,
            &nrecords_out);

    // Memory allocation for rdata before reading the table
    rdata.usemptinelt = (usemptinelt_t *) malloc((int) nrecords_out * sizeof(usemptinelt_t));
    rdata.nb = nrecords_out;

    // Read the table
    status = H5TBread_table(file_id, table_name, dst_size, dst_offset,
            dst_sizes, rdata.usemptinelt);
    return rdata;
}

semnodes_t readSemNodes(hid_t file_id, const char *table_name)
{
    herr_t status;
    hsize_t nfields_out;
    hsize_t nrecords_out;
    hid_t table_id;
    semnodes_t rdata;

    size_t dst_size = sizeof(semnode_t);
    size_t dst_offset[2] =
    { HOFFSET( semnode_t, shortname ), HOFFSET( semnode_t, index ) };

    size_t dst_sizes[2] =
    { sizeof(rdata.somnodes[0].shortname), sizeof(rdata.somnodes[0].index) };

    table_id = H5TBget_table_info(file_id, table_name, &nfields_out,
            &nrecords_out);

    // Memory allocation for rdata before reading the table
    rdata.somnodes = (semnode_t *) malloc((int) nrecords_out
            * sizeof(semnode_t));
    rdata.nbsomnodes = nrecords_out;

    // Read the table
    status = H5TBread_table(file_id, table_name, dst_size, dst_offset,
            dst_sizes, rdata.somnodes);
    return rdata;
}

ugroup_t readUGroup(hid_t group_id, const char* name)
{
    hid_t space;
    int ndims, i;
    hsize_t dims[1];
    ugroup_t rdata;
    herr_t status;

    rdata.name = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    strcpy(rdata.name, name);
    space = H5Dget_space(group_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    if(ndims!=0)
    {
      rdata.eltgroup = (int *) malloc(dims[0] * sizeof(int *));
      rdata.nbeltgroup = dims[0];
    }
    else
    {
      rdata.eltgroup = (int *) malloc( sizeof(int *));
      rdata.nbeltgroup = 1;
    }
    status = H5Dread(group_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.eltgroup);
    return rdata;
}

unstructured_mesh_t read_unstructured_mesh(hid_t file_id, const char* path)
{
    unstructured_mesh_t umesh;
    hid_t nodes_id, eltype_id, eltnode_id, loc_id;
    char *bufpath;
    char *bufpath2;
    hid_t groupGroup_id, group_id, grpGrp_id;
    children_t children;
    int i;

    // read nodes
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, NODES);
    nodes_id = H5Dopen1(file_id, bufpath);
    umesh.nodes = readNodes(nodes_id);
    H5Dclose(nodes_id);
    free(bufpath);

    // read element nodes
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, ELEMENT_NODES);
    eltnode_id = H5Dopen1(file_id, bufpath);
    umesh.element_nodes = readElementNodes(eltnode_id);
    H5Dclose(eltnode_id);
    free(bufpath);

    // read element types;
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, ELEMENTS);
    eltype_id = H5Dopen1(file_id, bufpath);
    umesh.elements = readElementTypes(eltype_id);
    H5Dclose(eltype_id);
    free(bufpath);

    // read groupGroup if exists
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, GROUPGROUP);

    umesh.groupgroups.nbgroupGroup = 0;
    if (H5Lexists(file_id, bufpath, H5P_DEFAULT) != FALSE)
    {
        children = read_children_name(file_id, bufpath);
        umesh.groupgroups.groupgroups = (groupgroup_t *) malloc(
                children.nbchild * sizeof(groupgroup_t));
        umesh.groupgroups.nbgroupGroup = children.nbchild;
        for (i = 0; i < children.nbchild; i++)
        {
            bufpath2
                    = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
            strcpy(bufpath2, bufpath);
            strcat(bufpath2, "/");
            strcat(bufpath2, children.childnames[i]);
            grpGrp_id = H5Dopen1(file_id, bufpath2);
            umesh.groupgroups.groupgroups[i] = readGroupGroup(grpGrp_id,
                    children.childnames[i]);
            H5Dclose(grpGrp_id);
            free(bufpath2);
        }
        free(children.childnames);
    }
    free(bufpath);

    // read group
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, GROUP);
    children = read_children_name(file_id, bufpath);
    umesh.groups.nbgroup = children.nbchild;
    umesh.groups.groups = (ugroup_t *) malloc(children.nbchild
            * sizeof(ugroup_t));
    for (i = 0; i < children.nbchild; i++)
    {
        bufpath2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        strcpy(bufpath2, bufpath);
        strcat(bufpath2, "/");
        strcat(bufpath2, children.childnames[i]);
        group_id = H5Dopen1(file_id, bufpath2);
        umesh.groups.groups[i] = readUGroup(group_id, children.childnames[i]);
        H5Dclose(group_id);
        free(bufpath2);
    }
    free(bufpath);

    // read selectorOnMesh/ptinelements
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, SELECTOR_ON_MESH);
    umesh.som_ptsinelements.nb = 0;
    umesh.som_nodes.nbsomnodes = 0;
    if (H5Lexists(file_id, bufpath, H5P_DEFAULT) != FALSE)
    {
        dimptsinelts_t dimsptelt;
        dimsptelt = readNbSemPtInElt(file_id, bufpath);
        int j;
        umesh.som_ptsinelements.ptsinelt = (usemptsinelt_t *) malloc(dimsptelt.nb * sizeof(usemptsinelt_t));
        for(j = 0; j < dimsptelt.nb ; j++)
        {
          umesh.som_ptsinelements.ptsinelt[j].usemptinelt = malloc(dimsptelt.nbptinelt[j] * sizeof(usemptinelt_t));
          bufpath2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
          strcpy(bufpath2, bufpath);
          strcat(bufpath2, "/");
          strcat(bufpath2, dimsptelt.name[j]);
          umesh.som_ptsinelements.ptsinelt[j] = readUSemPtInElt(file_id, bufpath2);
          strcpy(umesh.som_ptsinelements.ptsinelt[j].name,dimsptelt.name[j]);
          umesh.som_ptsinelements.ptsinelt[j].nb = dimsptelt.nbptinelt[j];
          free(bufpath2);
        }
        umesh.som_ptsinelements.nb = dimsptelt.nb;
    
    }
    free(bufpath);
    

    return umesh;
}

void print_unstructured_mesh(unstructured_mesh_t umesh)
{
    int i, j, offset;
    int element_type;

    offset = 0;
    printf("Name : %s\n", umesh.name);
    printf("Number of nodes : %i\n", umesh.nodes.nbnodes);
    for (i = 0; i < umesh.nodes.nbnodes; i++)
    {
        printf("Node n°%i : ", i);
        for (j = 0; j < 3; j++)
            printf("%f ", umesh.nodes.nodes[i][j]);
        printf("\n");
    }

    printf("\nNumber of elements : %i\n", umesh.elements.nbelttypes);
    for (i = 0; i < umesh.elements.nbelttypes; i++)
    {
        printf("Element n°%i , type : %i , ", i, umesh.elements.elttypes[i]);
        element_type = umesh.elements.elttypes[i];
        if (element_type == 1)
            for (j = 0; j < 2; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 2)
            for (j = 0; j < 3; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 11)
            for (j = 0; j < 3; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 12)
            for (j = 0; j < 6; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 13)
            for (j = 0; j < 4; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 14)
            for (j = 0; j < 8; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 101)
            for (j = 0; j < 4; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 102)
            for (j = 0; j < 5; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 103)
            for (j = 0; j < 6; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        else if (element_type == 104)
            for (j = 0; j < 8; j++)
            {
                printf("Node n°%i : %i ", j,
                        umesh.element_nodes.eltnodes[offset]);
                offset++;
            }
        printf("\n");
    }
    printf("\nNumber of groups : %i\n", umesh.groups.nbgroup);
    for (i = 0; i < umesh.groups.nbgroup; i++)
        printf(" Name : %s\n", umesh.groups.groups[i].name);

    printf("\nNumber of groupGroups : %i\n", umesh.groupgroups.nbgroupGroup);
    for (i = 0; i < umesh.groupgroups.nbgroupGroup; i++)
        printf(" Name : %s\n", umesh.groupgroups.groupgroups[i].name);

    printf("\nSelector on mesh / nodes ...\n");
    for (i = 0; i < umesh.som_nodes.nbsomnodes; i++)
        printf(" shortname = %s, index = %i\n",
                umesh.som_nodes.somnodes[i].shortname,
                umesh.som_nodes.somnodes[i].index);

    printf("\nSelector on mesh / point in elements ...\n");
    for (i = 0; i < umesh.som_ptsinelements.nb; i++){
      printf("name = %s : \n", umesh.som_ptsinelements.ptsinelt[i].name);
      for (j = 0; j < umesh.som_ptsinelements.ptsinelt[i].nb; j++)
        printf("    index = %i, v1 = %f, v2 = %f, v3 = %f\n",
                umesh.som_ptsinelements.ptsinelt[i].usemptinelt[j].index,
                umesh.som_ptsinelements.ptsinelt[i].usemptinelt[j].v1,
                umesh.som_ptsinelements.ptsinelt[i].usemptinelt[j].v2,
                umesh.som_ptsinelements.ptsinelt[i].usemptinelt[j].v3);
    }

}
