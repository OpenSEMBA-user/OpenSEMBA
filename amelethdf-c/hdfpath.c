#include <stdlib.h>
#include <string.h>

#include "hdfpath.h"

char* path_basename(char *path)
{
    char **buf;
    buf = rsplit(path);
    return buf[1];
}

char* path_dirname(char *path)
{
    char **buf;
    buf = rsplit(path);
    return buf[0];
}

// return TRUE if path exists
int path_exists(hid_t loc_id, char* path)
{
    int exists;
    if (H5Lexists(loc_id, path, H5P_DEFAULT) != FALSE)
    {
        exists = TRUE;
    }
    else
    {
        exists = FALSE;
    }
    return exists;
}

// return TRUE if path is absolute
int path_isabs(char *path)
{
    int isabs = FALSE;
    char* pch;
    pch = strchr(path, '/');
    if (pch != NULL)
        isabs = TRUE;
    return isabs;
}

H5I_type_t gettype(hid_t loc_id, char* path)
{
    hid_t obj_id;
    H5I_type_t type;
    herr_t status;

    obj_id = H5Oopen(loc_id, path, H5P_DEFAULT);
    if (obj_id < 0)
    {
        printf("Can't open %s\n", path);
    }
    type = H5Iget_type(obj_id);
    if (type == H5I_BADID)
        printf("Not valid type for %s\n", path);
    status = closetype(obj_id);
    if (status < 0)
        printf("Can't close type id for %s\n", path);
    return type;
}

// close an opened object
herr_t closetype(hid_t obj_id)
{
    herr_t status;
    H5I_type_t object_type;

    object_type = H5Iget_type(obj_id);
    status = -1;
    if (object_type == H5I_DATASET)
        status = H5Dclose(obj_id);
    else if (object_type == H5I_GROUP)
        status = H5Gclose(obj_id);
    return status;
}

// return TRUE if path is a leaf (table, dataset)
int path_isleaf(hid_t loc_id, char* path)
{
    int isleaf = FALSE;
    if (gettype(loc_id, path) == H5I_DATASET)
        isleaf = TRUE;
    return isleaf;
}

// return TRUE if path is a group
int path_isgroup(hid_t loc_id, char* path)
{
    int isgroup = FALSE;
    if (gettype(loc_id, path) == H5I_GROUP)
        isgroup = TRUE;
    return isgroup;
}

// remove separator "/" from the start and the end of string
// for "/foo/bar/" returns "foo/bar"
char * remove_sep(char *path)
{
    char *buf = "";
    int begin = 0;
    int end = 0;
    int i;
    char *pch;
    if (path_isabs(path))
        begin = 1;
    int pathsize = strlen(path) + 1 ;
    buf = (char *) malloc(pathsize * sizeof(char));
    pch = strchr(path, '/');
    while (pch != NULL)
    {
        if (strcmp(pch, "/") == 0)
            end = 1;
        pch = strchr(pch + 1, '/');
    }
    for (i = begin; i < (pathsize - end); i++)
    {
        buf[i - begin] = path[i];
    }
    //free(pch);
    return buf;
}

// join paths of an array of path
// for ["/foo", "bar"] returns "/foo/bar"
char* path_join(char **paths, int nbpath)
{
    int i;
    char *buf = "";
    int sizebuf = 1;
    for (i = 0; i < nbpath; i++)
        sizebuf = sizebuf + strlen(paths[i]);
    buf = (char *) malloc(sizebuf * sizeof(char));
    for (i = 0; i < nbpath; i++)
        strcat(buf, paths[i]);
    return buf;

}

char** rsplit(char *path)
{
    char **splitpath;
    int i;
    int nb = 1;
    char *pointer, *pointer2;
    char *sep =
    { "/" };
    char *buffer, *buffer2;

    buffer = strdup(path);
    buffer2 = strdup(path);
    buffer = remove_sep(buffer);
    buffer2 = remove_sep(buffer2);
    splitpath = (char **) malloc(2 * sizeof(char*));
    splitpath[0] = (char *) malloc((strlen(path)+1) * sizeof(char));

    pointer = strtok(buffer, sep);
    while (pointer != NULL)
    {
        pointer = strtok(NULL, sep);
        if (pointer != NULL)
        {
            nb++;
            splitpath[1] = strdup(pointer);
        }
    }
    pointer2 = strtok(buffer2, sep);
    splitpath[0] = (char *) malloc((strlen(pointer2)+1) * sizeof(char));
    splitpath[0] = strcat(splitpath[0], "/");
    splitpath[0] = strcat(splitpath[0], pointer2);
    if (nb > 1)
    {
        for (i = 0; i < nb - 2; i++)
        {
            pointer2 = strtok(NULL, sep);
            splitpath[0] = strcat(splitpath[0], "/");
            splitpath[0] = strcat(splitpath[0], pointer2);
        }
    }
    free(buffer2);
    free(buffer);
    return splitpath;
}

//lsplits a path and return the dirname and the basename
//for "/foo/bar/baz" returns "/foo" and "bar/baz"

char** lsplit(char *path)
{
    char **splitpath;
    int i;
    int nb = 1;
    char *pointer, *pointer2;
    char *sep =
    { "/" };
    char *buffer, *buffer2;

    buffer = strdup(path);
    buffer2 = strdup(path);
    buffer = remove_sep(buffer);
    buffer2 = remove_sep(buffer2);

    splitpath = (char **) malloc(2 * sizeof(char*));
    splitpath[0] = (char *) malloc((strlen(path)+1) * sizeof(char));

    pointer = strtok(buffer, sep);
    while (pointer != NULL)
    {
        pointer = strtok(NULL, sep);
        if (pointer != NULL)
        {
            nb++;
            splitpath[1] = strdup(pointer);
        }
    }
    pointer2 = strtok(buffer2, sep);
    splitpath[0] = strdup("/");
    splitpath[0] = strcat(splitpath[0], pointer2);
    pointer2 = strtok(NULL, sep);
    splitpath[1] = strdup(pointer2);
    if (nb > 1)
    {
        for (i = 1; i < nb - 1; i++)
        {
            pointer2 = strtok(NULL, sep);
            splitpath[0] = strcat(splitpath[0], "/");
            splitpath[0] = strcat(splitpath[0], pointer2);
        }
    }
    free(buffer2);
    free(buffer);
    return splitpath;
}

// splits a path and return the first element path
// for "/foo/bar/" returns "foo"

char * path_head(char *path)
{
    char **buf;
    buf = (char **) malloc(2 * sizeof(char*));
    buf[0] = strdup(path);
    buf[1] = strdup(path);
    buf = lsplit(path);
    return buf[0];
}

// returns true is subpath is in path
// for "/foo/bar" & "/foo" returns true

int path_contain(char *path, char *subpath)
{
    int i;
    int contain;
    contain = FALSE;
    if (strcmp(path, subpath) > 0)
        contain = TRUE;
    return contain;
}

// returns the nth element of path
// for ("/foo/bar/baz", 2) returns bar
char * path_element(char *path, int index, int back)
{
    char *res;
    char *buf;
    stringtab_t strbuf;
    int i;

    buf = (char *) malloc((strlen(path)+1) * sizeof(char));
    res = (char *) malloc((strlen(path)+1) * sizeof(char));
    if (strcmp(path, "") == 0)
        strcpy(res, path);
    else
    {
        strbuf = path_split(path);
        if (index > strbuf.nb)
            strcpy(res, "");
        else
        {
            if (back == FALSE)
            {
                strcpy(res, strbuf.chartab[index - 1]);
            }
            else
            {
                strcpy(res, strbuf.chartab[strbuf.nb - index - 1]);
            }
        }
    }
    free(strbuf.chartab[0]);
    free(strbuf.chartab);
    free(buf);
    return res;
}

stringtab_t path_split(char *path)
{
    int i;
    int nb = 1;
    char *pointer, *pointer2;
    char *sep =  "/" ;
    char *buffer, *buffer2;
    stringtab_t rdata;
    //buffer = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    //buffer2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    buffer = strdup(path);
    buffer2 = strdup(path);
    buffer = remove_sep(buffer);
    buffer2 = remove_sep(buffer2);

    pointer = strtok(buffer, sep);
    while (pointer != NULL)
    {
        pointer = strtok(NULL, sep);
        if (pointer != NULL)
        {
            nb++;
        }
    }
    rdata.chartab = (char **) malloc(nb * sizeof(char *));
    pointer2 = strtok(buffer2, sep);
    rdata.chartab[0] = strdup(pointer2);
    if (nb > 1)
    {
        for (i = 1; i < nb; i++)
        {
            pointer2 = strtok(NULL, sep);
            rdata.chartab[i] = strdup(pointer2);
        }
    }
    rdata.nb = nb;
    free(buffer2);
    free(buffer);
    return rdata;
}

//Return true if a path looks like a patter
//For "/foo/bar/baz" & "/foo/*/baz" return true

int path_like(char * path, char* pattern)
{
    int res;
    int res_inf = TRUE;
    int res_sup = TRUE;
    stringtab_t buf1;
    stringtab_t buf2;
    int i, nb_sup;
    int nb_inf = 1;

    buf1 = path_split(path);
    buf2 = path_split(pattern);

    for (i = 0; i < buf2.nb; i++)
    {
        if (strcmp(buf2.chartab[i], "*") != 0)
            nb_inf = i;
    }
    nb_sup = buf2.nb - nb_inf;
    for (i = 0; i < nb_inf; i++)
    {
        if (strcmp(buf2.chartab[i], buf1.chartab[i]) != 0)
            res_inf = FALSE;
    }
    if (res_inf == FALSE)
        res = FALSE;
    else
    {
        if (nb_sup > 0)
        {
            for (i = 0; i > nb_sup; i++)
            {
                if (strcmp(buf1.chartab[buf1.nb - i], buf2.chartab[buf2.nb - i])
                        != 0)
                    res_sup = FALSE;
            }
        }
        if ((res_inf == TRUE) && (res_sup == TRUE))
            res = TRUE;
    }
    return res;
}
