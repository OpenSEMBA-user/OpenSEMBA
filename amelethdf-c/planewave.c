#include <stdlib.h>
#include <string.h>

#include "planewave.h"

#define FALSE 0
#define TRUE 1

planewave_t read_planewave(hid_t file_id, const char* path)
{
    planewave_t pw;
    herr_t status;
    char *path2;

    pw.theta = read_float_attribute(file_id, path, A_THETA);
    pw.phi = read_float_attribute(file_id, path, A_PHI);
    pw.polarization = LINEAR;

    status = H5Aexists_by_name(file_id, path, A_LINEAR_POLARIZATION,
            H5P_DEFAULT);
    if (status < 0)
    {
        pw.elliptical_polarization_etheta = read_complex_attribute(file_id,
                path, A_ELLIPTICAL_POLARIZATION_ETHETA);
        pw.elliptical_polarization_ephi = read_complex_attribute(file_id, path,
                A_ELLIPTICAL_POLARIZATION_EPHI);
        pw.polarization = ELLIPTIC;
    }
    else
    {
        pw.linear_polarization = read_float_attribute(file_id, path,
                A_LINEAR_POLARIZATION);
    }
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(path2, path);
    strcat(path2, "/");
    strcat(path2, F_MAGNITUDE);
    pw.magnitude = read_floatingtype(file_id, path2);
    free(path2);
    return pw;
}

planewave_t clear_content_planewave(planewave_t pw)
{
    pw.theta = 0;
    pw.phi = 0;
    pw.linear_polarization = 0;
    pw.elliptical_polarization_etheta.re = 0;
    pw.elliptical_polarization_etheta.im = 0;
    pw.elliptical_polarization_ephi.re = 0;
    pw.elliptical_polarization_ephi.im = 0;
    pw.magnitude = clear_content_ft(pw.magnitude);
    return pw;

}

int islinear(planewave_t pw)
{
    int linear;
    linear = FALSE;
    if (pw.polarization == LINEAR)
        linear = TRUE;
    return linear;
}

int iselliptic(planewave_t pw)
{
    int elliptic;
    elliptic = FALSE;
    if (pw.polarization == ELLIPTIC)
        elliptic = TRUE;
    return elliptic;
}
