#include <stdlib.h>

#include "complextype.h"
#include <math.h>

#if __STDC_VERSION__ >= 199901L
//creal, cimag already defined in complex.h

__inline float_complex make_complex_float(float real, float imag)
{
   return real + imag * I;
}

__inline float abs_complex(float_complex a)
{
  return cabs(a);
}
#else

__inline float_complex make_complex_float(float real, float imag)
{
    float_complex z = {real, imag};
    return z;
}

__inline float abs_complex(float_complex a)
{
   float b = sqrt(a.re*a.re + a.im*a.im);
   return b;
}
#endif

// Create datatype
hid_t create_type_id(hid_t real_or_double)
{
    hid_t type_id;
    hsize_t type_size, two_type_size;
    herr_t status;

    type_size = H5Tget_size(real_or_double);
    two_type_size = type_size * 2;
    type_id = H5Tcreate(H5T_COMPOUND, two_type_size);
    status = H5Tinsert(type_id, "r", 0, real_or_double);
    status = H5Tinsert(type_id, "i", type_size, real_or_double);
    return type_id;
}

// Create the real datatype
hid_t create_real_type_id()
{
    hid_t real_type_id;
    real_type_id = create_type_id(H5T_NATIVE_FLOAT);
    return real_type_id;
}

// Create the double datatype
hid_t create_double_type_id()
{
    hid_t double_type_id;
    double_type_id = create_type_id(H5T_NATIVE_DOUBLE);
    return double_type_id;
}

// write the named complex type in a file (at the root)
herr_t write_complex_type(hid_t loc_id)
{
    herr_t status1, status2, status;
    hid_t real_type_id, double_type_id;

    status = 0;
    real_type_id = create_real_type_id();
    double_type_id = create_double_type_id();
    status1 = H5Tcommit1(loc_id, "/real_complex_type", real_type_id);
    if (status1 < 0)
    {
        printf("real complex type creation error !!");
        status = status1;
    }
    status2 = H5Tcommit1(loc_id, "/double_complex_type", double_type_id);
    if (status2 < 0)
    {
        printf("double complex type creation error !!");
        status = status2;
    }
    return status;
}

// create a complex float attribute
herr_t create_complex_attribute(hid_t loc_id, const char* path,
        const char* name, float_complex value)
{
    hid_t space_id, attr_id;
    hid_t real_type_id;
    hsize_t *dims;
    herr_t status;
    float * buf;

    real_type_id = create_real_type_id();
    space_id = H5Screate_simple(1,dims,NULL);
    attr_id = H5Acreate_by_name(loc_id,path,name,real_type_id,space_id,H5P_DEFAULT,H5P_DEFAULT, H5P_DEFAULT);
    buf[0] = crealf(value);
    buf[1] = cimagf(value);
    status = H5Awrite(attr_id,real_type_id,buf);
    return status;
}

// read a complex float attribute
float_complex read_complex_attribute(hid_t loc_id, const char* path,
        const char* name)
{
    float_complex value;
    float *buf;
    hid_t attr_id, real_type_id;
    ;
    hsize_t *dims;
    herr_t status;

    status = H5Aexists_by_name(loc_id, path, name, H5P_DEFAULT);
    if (status < 0)
    {
        printf("attribut %s does not exist \n", name);
    }
    real_type_id = create_real_type_id();
    attr_id = H5Aopen_by_name(loc_id, path, name, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Aread(attr_id, real_type_id, buf);
    if (status < 0)
    {
        printf("Can't read attribute : %s\n", name);
    }
    value.re = buf[0]; value.im=buf[1];
    return value;
}

// read a complex float dataset

float_complex *read_complex_dataset(hid_t loc_id, const char* path)
{
    float_complex *values;
    int rank;
    int i;
    hsize_t *dims;
    int length;
    size_t type_size;
    float * buf;
    hid_t real_id_type;
    herr_t status;

    real_id_type = create_real_type_id();
    status = H5LTget_dataset_ndims(loc_id, path, &rank);
    if (status < 0)
    {
        printf("Can't read rank \n");
    }
    dims = (hsize_t *) malloc(rank * sizeof(hsize_t));
    status = H5LTget_dataset_info(loc_id, path, dims, NULL, NULL);
    if (status < 0)
    {
        printf("Can't read dataset info\n");
    }
    length = dims[0];
    for (i = 1; i < rank; i++)
        length = length * dims[i];
    buf = (float *) malloc(2 * length * sizeof(float));
    status = H5LTread_dataset(loc_id, path, real_id_type, buf);
    if (status < 0)
    {
        printf("Can't read dataset\n");
    } 
    values = (float_complex *) malloc(length*sizeof(float_complex));
    int j = 0;
    for (i = 0; i < length; i++)
    {
        values[i].re = buf[j];
        values[i].im = buf[j + 1];
        j = j + 2;
    }
    free(buf);
    free(dims);
    return values;
}

// Write a 1D complex dataset

herr_t write_complexe_1D_dataset(hid_t loc_id, const char* path,
        float_complex* values, int nbvalues)
{
    int i,rank = 1;
    hsize_t dims[1];
    hid_t dataspace_id, dset_id, dtr_id, dti_id;
    size_t type_size;
    hid_t type_id;
    herr_t status, status_2;
    float *real_part, *imag_part;

    status_2 = 0;
    real_part = (float *)malloc(nbvalues * sizeof(float));
    imag_part = (float *)malloc(nbvalues * sizeof(float));
    for( i=0;i<nbvalues;i++)
    {
        real_part[i] = crealf(values[i]);
        imag_part[i] = cimagf(values[i]);
    }
    type_id = create_real_type_id();
    dims[0] = nbvalues;
    dataspace_id = H5Screate_simple(rank, dims, NULL);
    dset_id = H5Dcreate(loc_id,path,type_id,dataspace_id,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //Create datatype for writing
    type_size = H5Tget_size(H5T_NATIVE_FLOAT);
    dtr_id = H5Tcreate(H5T_COMPOUND,type_size);
    status = H5Tinsert(dtr_id,"r",0, H5T_NATIVE_FLOAT);
    if(status<0)
    {
        printf("Can insert real part \n");
        status_2=-1;
    }
    dti_id = H5Tcreate(H5T_COMPOUND,type_size);
    status = H5Tinsert(dti_id,"i",0, H5T_NATIVE_FLOAT);
    if(status<0)
    {
        printf("Can insert imaginary part \n");
        status_2=-1;
    }
    //write data
    status = H5Dwrite(dset_id,dtr_id,H5S_ALL,H5S_ALL,H5P_DEFAULT,real_part);
    if(status<0)
    {
        printf("Can write real part \n");
        status_2=-1;
    }
    status = H5Dwrite(dset_id,dti_id,H5S_ALL,H5S_ALL,H5P_DEFAULT,imag_part);
    if(status<0)
    {
        printf("Can write imaginary part \n");
        status_2=-1;
    }
    status = H5Tclose(dtr_id);
    status = H5Tclose(dti_id);
    status = H5Tclose(dset_id);
    status = H5Tclose(dataspace_id);
    free(real_part);
    free(imag_part);
    return status_2;
}
