#ifndef PLANEWAVE_H
#define PLANEWAVE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "floatingtype.h"
#define C_PLANE_WAVE "planeWave"
#define F_MAGNITUDE "magnitude"
#define A_THETA "theta"
#define A_PHI "phi"
#define A_LINEAR_POLARIZATION "linearPolarization"
#define A_ELLIPTICAL_POLARIZATION_ETHETA "ellipticalPolarizationETheta"
#define A_ELLIPTICAL_POLARIZATION_EPHI "ellipticalPolarizationEPhi"

#define LINEAR 1
#define ELLIPTIC 2

typedef struct
{
        float theta;
        float phi;
        float linear_polarization;
        int polarization;
        float_complex elliptical_polarization_etheta;
        float_complex elliptical_polarization_ephi;
        floatingtype_t magnitude;
} planewave_t;

planewave_t read_planewave(hid_t file_id, const char* path);
planewave_t clear_content_planewave(planewave_t pw);
int islinear(planewave_t pw);
int iselliptic(planewave_t pw);

#ifdef __cplusplus
}
#endif

#endif // PLANEWAVE_H
