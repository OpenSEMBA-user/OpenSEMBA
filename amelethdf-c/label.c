#include <stdlib.h>

#include "label.h"

label_t read_label(hid_t file_id, const char* path)
{
    label_t rdata;
    hsize_t n;
    size_t l;

    rdata.name = strdup(path);
    get_dataset_dims(file_id, path, &rdata.nb, &n, &l);
    rdata.values = read_string_dataset2(file_id, path,l, rdata.nb);
    return rdata;
}


void print_label(label_t label)
{
    hsize_t i;

    if (label.nb != 0)
    {
        printf("   Name : %s\n", label.name);
        for (i = 0; i < label.nb; i++)
            printf("      %s\n", label.values[i]);
    }
}
