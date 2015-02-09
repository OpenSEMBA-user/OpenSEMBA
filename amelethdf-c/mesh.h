#ifndef MESH_H
#define MESH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"
#define STRUCTURED         0
#define UNSTRUCTURED       1
#define C_MESH             "/mesh"
#define NODES              "/nodes"
#define ELEMENTS           "/elementTypes"
#define ELEMENT_NODES      "/elementNodes"
#define GROUP              "/group"
#define GROUPGROUP         "/groupGroup"
#define SELECTOR_ON_MESH   "/selectorOnMesh"
#define L_ELEMENTS         "/elements"
#define G_CARTESIAN_GRID   "/cartesianGrid"
#define G_NORMAL           "/normal"
#define D_X                "/x"
#define D_Y                "/y"
#define D_Z                "/z"
#define A_TYPE             "type"

typedef struct
{
        char** groupGroupnames;
        int nbeltgroupGroup;
        char* name;
} groupgroup_t;

typedef struct
{
        groupgroup_t *groupgroups;
        int nbgroupGroup;
} groupgroups_t;

typedef struct
{
  hsize_t* nbptinelt;
  char** name;
  int nb;
} dimptsinelts_t;

int meshtype(hid_t loc_id, const char * path);
groupgroup_t readGroupGroup(hid_t grpgrp_id, const char* name);
dimptsinelts_t readNbSemPtInElt(hid_t file_id, const char *path);

#ifdef __cplusplus
}
#endif

#endif // MESH_H
