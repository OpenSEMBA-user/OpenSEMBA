#ifndef UNSTRUCTUREDMESH_H
#define UNSTRUCTUREDMESH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesh.h"
typedef struct
{
        float ** nodes;
        int nbnodes;
} nodes_t;

typedef struct
{
        int * elttypes;
        int nbelttypes;
} elttypes_t;

typedef struct
{
        int * eltnodes;
        int nbeltnodes;
} eltnodes_t;


typedef struct
{
        int index;
        float v1;
        float v2;
        float v3;
} usemptinelt_t;

typedef struct
{
        usemptinelt_t *usemptinelt;
        int nb;
        char name[30];
} usemptsinelt_t;

typedef struct
{
       usemptsinelt_t *ptsinelt;
       int nb;
} usemptsinelts_t;

typedef struct
{
        char shortname[30];
        int index;
} semnode_t;

typedef struct
{
        semnode_t *somnodes;
        int nbsomnodes;
} semnodes_t;

typedef struct
{
        int* eltgroup;
        int nbeltgroup;
        char* name;
} ugroup_t;

typedef struct
{
        ugroup_t* groups;
        int nbgroup;
} ugroups_t;

typedef struct
{
        char * name;
        nodes_t nodes;
        elttypes_t elements;
        eltnodes_t element_nodes;
        ugroups_t groups;
        groupgroups_t groupgroups;
        semnodes_t som_nodes;
        usemptsinelts_t som_ptsinelements;
} unstructured_mesh_t;

nodes_t readNodes(hid_t node_id);
elttypes_t readElementTypes(hid_t eltypes_id);
eltnodes_t readElementNodes(hid_t elnodes_id);
usemptsinelt_t readUSemPtInElt(hid_t file_id, const char *table_name);
semnodes_t readSemNodes(hid_t file_id, const char *table_name);
ugroup_t readUGroup(hid_t group_id, const char* name);
unstructured_mesh_t read_unstructured_mesh(hid_t file_id, const char* path);
void print_unstructured_mesh(unstructured_mesh_t umesh);

#ifdef __cplusplus
}
#endif

#endif // UNSTRUCTUREDMESH_H
