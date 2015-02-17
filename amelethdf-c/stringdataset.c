#include <hdf5.h>
#include <hdf5_hl.h>
#include <stdlib.h>
#include <string.h>
#include "stringdataset.h"
#include <ctype.h>


// Read a long string dataset
char ** read_string_dataset2(hid_t file_id, const char * path, size_t length, hsize_t mn)
{
    char **rdata;
    herr_t status;
    hid_t dset, memtype;
    int i;

    length++; // make a space for the null terminator
    dset = H5Dopen(file_id, path, H5P_DEFAULT);

    rdata = (char **) malloc(mn * sizeof(char*));
    rdata[0] = (char *) malloc(mn * length * sizeof(char));
    for (i = 1; i < mn; i++)
        rdata[i] = rdata[0] + i * length;
    memtype = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(memtype, length);
    status = H5Dread(dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]);
    H5Tclose(memtype);
    H5Dclose(dset);
    return rdata;
}

// Read a (m x n) string datset (rows x columns) of l characters
char *** read_string_dataset1(hid_t file_id, const char * path, int l, int m,
        int n)
{
    char **buf;
    char ***data;
    int i, j, k, offset;

    buf = (char **) malloc(m * n * sizeof(char*));
    buf[0] = (char *) malloc(l * m * n * sizeof(char));
    for (i = 1; i < m * n; i++)
        buf[i] = buf[0] + i * l;

    data = (char ***) malloc(n * sizeof(char**));
    for (i = 0; i < n; i++)
    {
        data[i] = (char**) malloc(n * m * sizeof(char*));
        data[i][0] = (char *) malloc(n * m * l * sizeof(char));
        for (j = 1; j < m; j++)
            data[i][j] = data[i][0] + j * l;
    }
    buf = read_string_dataset2(file_id, path, l, m * n);
    offset = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            data[i][j] = strdup(buf[j + offset]);
        }
        offset = offset + m;
    }
    free(buf[0]);
    free(buf);
    return data;
}

// Reads the length of strings contains in a dataset
// The dataset must be a string dataset (no check)
int *get_dataset_lmn(hid_t file_id, char* path)
{
    int *lmn;
    H5T_class_t type_class;
    int m = 0, n = 0;
    herr_t status;
    size_t type_size;
    hsize_t dims[2];

    lmn = (int *) malloc(3 * sizeof(int));
    dims[0] = 0;
    dims[1] = 0;
    status = H5LTget_dataset_info(file_id, path, dims, &type_class, &type_size);
    m = dims[0];
    n = dims[1];
    lmn[0] = type_size;
    lmn[1] = n;
    lmn[2] = m;
    return lmn;
}

void get_dataset_dims(hid_t file_id, const char* path, hsize_t *m, hsize_t *n, size_t *l)
{
    H5T_class_t type_class;
    herr_t status;
    hsize_t dims[2] = {1, 1};

    status = H5LTget_dataset_info(file_id, path, dims, &type_class, l);
    *m = dims[0];
    *n = dims[1];

    if (status < 0)
    {
        *m = 0;
        *n = 0;
        *l = 0;
    }
}


children_t read_string_vector(hid_t file_id, char* path)
{
    char ***vector;
    int *lmn;
    int i, j;
    children_t child;

    lmn = (int *) malloc(3 * sizeof(int));
    lmn = get_dataset_lmn(file_id, path);
    if ((lmn[1] != 0) && (lmn[1] != 1))
        printf("%s must be a (n x 1) or (n) dataset\n", path);
    vector = (char ***) malloc(sizeof(char **));
    for (i = 0; i < 1; i++)
    {
        vector[i] = (char**) malloc(lmn[2] * sizeof(char*));
        vector[i][0] = (char *) malloc(lmn[2] * lmn[0] * sizeof(char));
        for (j = 1; j < lmn[2]; j++)
            vector[i][j] = vector[i][0] + j * lmn[0];
    }
    vector = read_string_dataset1(file_id, path, lmn[0], lmn[2], 1);
    child.childnames = (char **) malloc(lmn[2] * sizeof(char *));
    child.childnames[0] = (char*) malloc(lmn[0] * lmn[2] * sizeof(char));
    for (i = 0; i < lmn[2]; i++)
        strcpy(child.childnames[i], vector[0][i]);
    child.nbchild = lmn[2];
    return child;
}

// return the index of an element
int str_index(char ** arr, char *aelement, int nbel)
{
    int index = -1;
    int i;

    for (i = 0; i < nbel; i++)
        if (strcmp(arr[i], aelement) == 0)
            index = i;
    return index;
}

// write a nd string dataset
void write_string_dataset(hid_t file_id, char* path, int values_len, int rank,
        hsize_t* dims, char** values)
{
    hid_t ds_id, dset_id, filetype, memtype;
    herr_t err;

    // Create string data type
    filetype = H5Tcopy(H5T_FORTRAN_S1);
    err = H5Tset_size(filetype, values_len - 1);
    memtype = H5Tcopy(H5T_C_S1);
    err = H5Tset_size(memtype, values_len);

    // Create the data space
    ds_id = H5Screate_simple(rank, dims, NULL);

    // Create the dataset
    dset_id = H5Dcreate(file_id, path, filetype, ds_id, H5P_DEFAULT,
            H5P_DEFAULT, H5P_DEFAULT);
    // Write data
    err = H5Dwrite(dset_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, values);

    H5Tclose(filetype);
    H5Tclose(memtype);
    H5Dclose(dset_id);
    H5Sclose(ds_id);
}

char *remove_space(char *name)
{
      char *buf = name, *output = name;
      int i = 0, count = 0;

      if (name)
      {

            for (buf = name; *buf && isspace(*buf); ++buf)
                  ;
            if (name != buf)
                  memmove(name, buf, buf - name);

            while (*buf)
            {
                  if (isspace(*buf) && count)
                        buf++;
                  else
                  {
                        if (!isspace(*buf))
                              count = 0;
                        else
                        {
                              *buf = ' ';
                              count = 1;
                        }
                        output[i++] = *buf++;
                  }
            }
            output[i] = '\0';

            while (--i >= 0)
            {
                  if (!isspace(output[i]))
                        break;
            }
            output[++i] = '\0';
      }
      return name;
}
