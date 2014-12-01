#include <stdlib.h>
#include <string.h>

#include "floatingtype.h"

floatingtype_t read_floatingtype(hid_t loc_id, const char *path)
{
    floatingtype_t ft;

    ft.floatingtype = get_type_ft(loc_id, path);
    if (ft.floatingtype == E_SINGLE_REAL)
        ft.singlereal = read_singlereal(loc_id, path);
    else if (ft.floatingtype == E_SINGLE_COMPLEX)
        ft.singlecomplex = read_singlecomplex(loc_id, path);
    else if (ft.floatingtype == E_VECTOR)
        ft.vector = read_vector(loc_id, path);
    else if (ft.floatingtype == E_DATA_SET)
        ft.dataset = read_dataset(loc_id, path);
    else if (ft.floatingtype == E_ARRAY_SET)
        ft.arrayset = read_arrayset(loc_id, path);
    return ft;
}

int get_type_ft(hid_t loc_id, const char* path)
{
    char* buf;
    int type;

    type = -1;

    buf = (char *) malloc(200 * sizeof(char));
    buf = read_string_attribute(loc_id, path, A_FLOATING_TYPE);
    if (strcmp(buf, V_SINGLE_REAL) == 0)
        type = E_SINGLE_REAL;
    else if (strcmp(buf, V_SINGLE_COMPLEX) == 0)
        type = E_SINGLE_COMPLEX;
    else if (strcmp(buf, V_DATA_SET) == 0)
        type = E_DATA_SET;
    else if (strcmp(buf, V_VECTOR) == 0)
        type = E_VECTOR;
    else if (strcmp(buf, V_ARRAY_SET) == 0)
        type = E_ARRAY_SET;
    else
        printf("Can't get floating type for %s\n", path);
    free(buf);
    return type;
}

floatingtype_t clear_content_ft(floatingtype_t ft)
{
    ft.floatingtype = 0;
    ft.singlereal = clear_content_singlereal(ft.singlereal);
    ft.singlecomplex = clear_content_singlecomplex(ft.singlecomplex);
    ft.vector = vector_clear_content(ft.vector);
    ft.dataset = clear_content_dataset(ft.dataset);
    ft.arrayset = clear_content_ars(ft.arrayset);

    return ft;
}

//Series of test functions
int issinglereal(floatingtype_t ft)
{
    int singlereal = FALSE;
    if (ft.floatingtype == E_SINGLE_REAL)
        singlereal = TRUE;
    return singlereal;
}

int issinglecomplex(floatingtype_t ft)
{
    int singlecomplex = FALSE;
    if (ft.floatingtype == E_SINGLE_COMPLEX)
        singlecomplex = TRUE;
    return singlecomplex;
}

int isvector(floatingtype_t ft)
{
    int vect = FALSE;
    if (ft.floatingtype == E_VECTOR)
        vect = TRUE;
    return vect;
}

int isdataset(floatingtype_t ft)
{
    int ds = FALSE;
    if (ft.floatingtype == E_DATA_SET)
        ds = TRUE;
    return ds;
}

int isarrayset(floatingtype_t ft)
{
    int ars = FALSE;
    if (ft.floatingtype == E_ARRAY_SET)
        ars = TRUE;
    return ars;
}
