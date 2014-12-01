#ifndef LABEL_H
#define LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stringdataset.h"

typedef struct
{
        char* name;
        char** values;
        hsize_t nb;
} label_t;

label_t read_label(hid_t file_id, const char* path);
void print_label(label_t label);

#ifdef __cplusplus
}
#endif

#endif // LABEL_H
