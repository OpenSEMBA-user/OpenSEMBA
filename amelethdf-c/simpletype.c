#include <hdf5.h>
#include <hdf5_hl.h>
#include "simpletype.h"
#include <string.h>
#include <amelethdf.h>
#include <stdlib.h>

single_t read_single(hid_t loc_id, const char* path)
{
    single_t single;
    herr_t status;
    htri_t test;

    single.label = (char*) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(single.label, "");
    test = H5Aexists_by_name(loc_id, path, A_LABEL, H5P_DEFAULT);
    if (test > 0)
    {
        strcpy(single.label, read_string_attribute(loc_id, path, A_LABEL));
    }

    single.physical_nature = (char*) malloc(ABSOLUTE_PATH_NAME_LENGTH
            * sizeof(char));
    strcpy(single.physical_nature, "");
    test = H5Aexists_by_name(loc_id, path, A_PHYSICAL_NATURE, H5P_DEFAULT);
    if (test > 0)
    {
        strcpy(single.physical_nature, read_string_attribute(loc_id, path,
                A_PHYSICAL_NATURE));
    }

    single.unit = (char*) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(single.unit, "");
    test = H5Aexists_by_name(loc_id, path, A_UNIT, H5P_DEFAULT);
    if (test > 0)
    {
        strcpy(single.unit, read_string_attribute(loc_id, path, A_UNIT));
    }

    single.comment = (char*) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(single.comment, "");
    test = H5Aexists_by_name(loc_id, path, A_COMMENT, H5P_DEFAULT);
    if (test > 0)
    {
        strcpy(single.comment, read_string_attribute(loc_id, path, A_COMMENT));
    }
    return single;
}

single_t clear_content_single(single_t single)
{

    free(single.label);
    free(single.comment);
    free(single.physical_nature);
    free(single.unit);

    return single;
}

singlereal_t clear_content_singlereal(singlereal_t singlereal)
{
    singlereal.single = clear_content_single(singlereal.single);
    singlereal.value = 0.;
    return singlereal;
}

singlecomplex_t clear_content_singlecomplex(singlecomplex_t singlecomplex)
{
    singlecomplex.single = clear_content_single(singlecomplex.single);
    singlecomplex.value.re = 0.;
    singlecomplex.value.im = 0.;
    return singlecomplex;
}

singlereal_t read_singlereal(hid_t loc_id, const char *path)
{
    herr_t status;
    singlereal_t singlereal;
    float *rdata;

    singlereal.single = read_single(loc_id, path);
    status = H5LTget_attribute_float(loc_id, path, A_VALUE, rdata);
    singlereal.value = rdata[0];
    return singlereal;
}

singlecomplex_t read_singlecomplex(hid_t loc_id, const char *path)
{
    herr_t status;
    singlecomplex_t singlecomplex;

    singlecomplex.single = read_single(loc_id, path);
    singlecomplex.value = read_complex_attribute(loc_id, path, A_VALUE);
    return singlecomplex;
}

char* single_to_string(single_t single)
{
    char *s;
    s = (char *) malloc(180 * sizeof(char));
    strcpy(s, "");
    strcat(s, "Label : ");
    strcat(s, single.label);
    strcat(s, " , Physical nature : ");
    strcat(s, single.physical_nature);
    strcat(s, " , Unit : ");
    strcat(s, single.unit);
    strcat(s, " , Comment : ");
    strcat(s, single.comment);
    strcat(s, "\n");
    return s;
}

char* singlereal_to_string(singlereal_t singlereal)
{
    char* s;
    char* buf;
    s = single_to_string(singlereal.single);
    sprintf(buf, " %f", singlereal.value);
    strcat(s, " , Value : ");
    strcat(s, buf);
    return s;
}

char* singlecomplex_to_string(singlecomplex_t singlecomplex)
{
    char* s;
    char* buf;
    s = single_to_string(singlecomplex.single);
    sprintf(buf, " %f + i %f ", crealf(singlecomplex.value), cimagf(
            singlecomplex.value));
    strcat(s, " , Value : ");
    strcat(s, buf);
    return s;
}

herr_t make_single(hid_t loc_id, const char* path, single_t single)
{
    int slen;
    herr_t status, status_2 = 0;
    status = H5LTset_attribute_string(loc_id, path, A_LABEL, single.label);
    if (status < 0)
    {
        printf("Can't write label for %s\n", path);
        status_2 = status;
    }
    status = H5LTset_attribute_string(loc_id, path, A_PHYSICAL_NATURE,
            single.physical_nature);
    if (status < 0)
    {
        printf("Can't write physical nature for %s\n", path);
        status_2 = status;
    }
    status = H5LTset_attribute_string(loc_id, path, A_UNIT, single.unit);
    if (status < 0)
    {
        printf("Can't write unit for %s\n", path);
        status_2 = status;
    }
    status = H5LTset_attribute_string(loc_id, path, A_COMMENT, single.comment);
    if (status < 0)
    {
        printf("Can't write comment for %s\n", path);
        status_2 = status;
    }
    return status_2;
}
