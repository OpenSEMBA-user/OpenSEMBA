#include "linkoutputrequest.h"
#include "hdfpath.h"

// Reads a (subject/object) of link/outputRequest
link_t read_link(hid_t loc_id, char * path)
{
    int ok;
    link_t link;

    link.subject = read_string_attribute(loc_id, path, A_SUBJECT);
    link.object = read_string_attribute(loc_id, path, A_OBJECT);
    return link;
}

// Return the type of link

int get_type_link(link_t link)
{
    int type;
    type = -1;
    if (strcmp(link.subject, "/physicalModel/conductingMaterial") == 0)
        type = DATA_ON_MESH;
    else if (path_like(link.subject, "/physicalModel/*/*"))
        type = DATA_ON_MESH;
    else if (path_like(link.subject, "/electromagneticSource/*/*"))
        type = DATA_ON_MESH;
    else if (path_like(link.subject, "/mesh/*/*/group/*/"))
        type = DATA_ON_MESH;
    else if (path_like(link.subject, "/mesh/*/*/groupgroup/*/"))
        type = DATA_ON_MESH;
    else if (path_like(link.subject, "/mesh/*/*"))
        type = DATA_ON_MESH;

    return type;
}

int isdataonmesh(link_t link)
{
    int itis = FALSE;

    if (get_type_link(link) == DATA_ON_MESH)
        itis = TRUE;
    return itis;

}
