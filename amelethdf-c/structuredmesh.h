#ifndef STRUCTUREDMESH_H
#define STRUCTUREDMESH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesh.h"
typedef struct
{
        float * nodes;
        int nbnodes;
} axis_t;

typedef struct
{
        unsigned int *imin;
        unsigned int *jmin;
        unsigned int *kmin;
        unsigned int *imax;
        unsigned int *jmax;
        unsigned int *kmax;
        unsigned int nbelt;
        char *type;
        char *entityType;
        char *name;
} sgroup_t;

typedef struct
{
        char ** normals;
        unsigned int nbelt;
} snorm_t;

typedef struct
{
        sgroup_t* groups;
        int nbgroup;
} sgroups_t;

typedef struct
{
        int imin;
        int jmin;
        int kmin;
        int imax;
        int jmax;
        int kmax;
        float v1;
        float v2;
        float v3;
} ssemptinelt_t;

typedef struct
{
        ssemptinelt_t *ssemptinelt;
        int nb;
        char name[30];
} ssemptsinelt_t;

typedef struct
{
       ssemptsinelt_t *ptsinelt;
       int nb;
} ssemptsinelts_t;

typedef struct
{
        char * name;
        axis_t x;
        axis_t y;
        axis_t z;
        sgroups_t groups;
        groupgroups_t groupgroups;
        ssemptsinelts_t som_ptsinelements;
} structured_mesh_t;

axis_t readAxis(hid_t node_id, const char *axisname);
sgroup_t readSGroup(hid_t group_id, const char* name);
snorm_t readNormals(hid_t loc_id);
char* readAttributes(hid_t group_id, const char* attr_name);
structured_mesh_t read_structured_mesh(hid_t file_id, const char* path);
void print_structured_mesh(structured_mesh_t smesh);
ssemptsinelt_t readSSemPtInElt(hid_t file_id, const char *table_name);

#ifdef __cplusplus
}
#endif

#endif // STRUCTUREDMESH_H
