#include <string.h>
#include <stdlib.h>
#include "amelethdf.h"
#include "set.h"

// Return TRUE if aelement is in aset
int in(set_t aset, char* aelement)
{
    int i;
    int present = FALSE;
    for (i = 0; i < aset.number_of_element; i++)
    {
        if (strcmp(aset.array[i], aelement) == 0)
            present = TRUE;
    }
    return present;
}

// Add aelement to aset
set_t add(set_t aset, char* aelement)
{
    hsize_t id = 0;
    if (!index_in_set(aset, aelement, &id))
    {
        aset.array[aset.number_of_element] = strdup(aelement);
        aset.number_of_element++;
    }
    return aset;
}

// Return the index of first occurrence of an element^M
int index_in_set(set_t aset, char *aelement, hsize_t *index)
{
    int present = FALSE;
    hsize_t i;

    for (i = 0; i < aset.number_of_element; i++)
    {
        if (strcmp(aelement, aset.array[i]) == 0)
        {
            *index = i;
            present = TRUE;
            break;
        }
    }
    return present;
}

// Get ith from aset
char* get(set_t aset, int id)
{
    char* result;
    result = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(result, aset.array[id]);
    return result;
}

// Return the index of an element
int get_index(set_t aset, char* aelement)
{
    int i, index = -1;
    for (i = 0; i < aset.number_of_element; i++)
        if (strcmp(aelement, aset.array[i]) == 0)
            index = i;
    return index;
}

set_t clean(set_t aset)
{
    char* buf;
    buf = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(buf, "");
    if (aset.array != NULL)
        strcpy(aset.array[0], buf);
    aset.number_of_element = 0;
    return aset;
}
