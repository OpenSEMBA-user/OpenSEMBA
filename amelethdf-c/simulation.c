#include <stdlib.h>

#include "simulation.h"

simulation_t read_simulation(hid_t file_id, const char* sim_path)
{
    simulation_t rdata;
    char* path;
    hsize_t n;
    size_t l;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    // inputs
    rdata.name = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(rdata.name, sim_path);
    strcpy(path, sim_path);
    strcat(path, INPUTS);

    if (H5LTfind_dataset(file_id, path) == 0)
    {
        get_dataset_dims(file_id, path, &rdata.nb_input, &n, &l);
        rdata.inputs = read_string_dataset2(file_id, path, l, rdata.nb_input);
    }

    // outputs
    strcpy(path, sim_path);
    strcat(path, OUTPUTS);
    if (H5LTfind_dataset(file_id, path) == 0)
    {
        get_dataset_dims(file_id, path, &rdata.nb_output, &n, &l);
        rdata.outputs = read_string_dataset2(file_id, path, l, rdata.nb_output);
    }
    free(path);
    return rdata;
}

// return the nb of input or output and the type_size
int* read_nb_inputs_outputs(hid_t file_id, const char* path)
{
    int* nb;
    hsize_t dims[1];
    H5T_class_t type_class;
    size_t type_size;
    herr_t status;

    status = H5LTget_dataset_info(file_id, path, dims, &type_class, &type_size);
    if (status < 0)
        printf("can't read the number of inputs for %s\n", path);
    nb = (int*) malloc(2 * sizeof(int));
    nb[0] = dims[0];
    nb[1] = type_size;
    return nb;
}

char ** read_inputs_outputs(hid_t file_id, const char* path)
{
    char*** in_out;
    hsize_t dims[1];
    H5T_class_t type_class;
    size_t type_size;
    herr_t status;
    int i, j;

    status = H5LTget_dataset_info(file_id, path, dims, &type_class, &type_size);
    if (status < 0)
        printf("can't read the number of inputs for %s\n", path);

    //	in_out = (char **)malloc(dims[0]*sizeof(char*));
    //	in_out[0] = (char *)malloc(dims[0]*type_size*sizeof(char));
    //	for(i=1;i<dims[0];i++)
    //				in_out[i]=in_out[0]+ type_size*i;
    int l = (int) type_size;
    int m = (int) dims[0];
    int n = 1;
    in_out = (char ***) malloc(n * sizeof(char**));
    for (i = 0; i < n; i++)
    {
        in_out[i] = (char**) malloc(n * m * sizeof(char*));
        for (j = 1; j < m; j++)
            in_out[i][j] = in_out[i][0] + j * l;
    }
    in_out = read_string_dataset1(file_id, path, l, m, n);
    return in_out[0];
}

void print_simulation(simulation_t sim)
{
    int i;
    printf("Name : %s\n", sim.name);
    if (sim.nb_input != 0)
    {
        printf("    Inputs :\n");
        for (i = 0; i < sim.nb_input; i++)
            printf("        %s\n", sim.inputs[i]);
    }
    if (sim.nb_output != 0)
    {
        printf("    Outputs :\n");
        for (i = 0; i < sim.nb_output; i++)
            printf("        %s\n", sim.outputs[i]);
    }

}
