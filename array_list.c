#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "array_list.h"

struct array_list *new_array_list()
{
    struct array_list *list = malloc(sizeof(struct array_list));
    list->content = malloc(sizeof(char*) * MAX_SIZE);
    for(int i = 0; i < MAX_SIZE; ++i) {
        list->content[i] = NULL;
    }
    list->capacity = MAX_SIZE;
    list->size = 0;
    return list;
}

void expand_array_list(struct array_list *list) {
    // allocate a list of twice the size of the previous
    int new_capacity = (list->capacity) * 2;
    list->capacity = new_capacity;

    // copy values
    list->content = realloc(list->content, sizeof(char*) * new_capacity);

    // initialize remaining slots to NULL
    for(int i = list->size; i < new_capacity; ++i) {
        list->content[i] = NULL;
    }
}

// appends to list, if len exceeds size then size is increased by 2 times
void array_list_add_to_end(struct array_list *list, char *str)
{
    if ((list->size + 1) > list->capacity)
    {
        // reallocate to a larger array
        expand_array_list(list);
    }
    // append to list content
    list->content[list->size] = malloc(sizeof(char) * ((int) strlen(str) + 1));
    strcpy(list->content[list->size], str);
    ++(list->size);
}

void destroy_array_list(struct array_list *list) 
{
    for (int i = 0; i < list->size; ++i) {
        if (list->content[i] != NULL) free(list->content[i]);
    }
    free(list->content);
    free(list);
}
