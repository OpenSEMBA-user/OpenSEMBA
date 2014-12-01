#ifndef SIMULATION_H
#define SIMULATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stringdataset.h"
#define C_SIMULATION "/simulation"
#define INPUTS "/inputs"
#define OUTPUTS "/outputs"
#define PARAMETERS "/parameters"

typedef struct
{
        char* name;
        char** inputs;
        char** outputs;
        hsize_t nb_input;
        hsize_t nb_output;
} simulation_t;

simulation_t read_simulation(hid_t file_id, const char* sim_path);
int* read_nb_inputs_outputs(hid_t file_id, const char* path);
char ** read_inputs_outputs(hid_t file_id, const char* path);
void print_simulation(simulation_t sim);

#ifdef __cplusplus
}
#endif

#endif // SIMULATION_H
