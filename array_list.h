

#define MAX_SIZE 8

struct array_list
{
    char **content;
    int capacity;
    int size;
};

struct array_list *new_array_list();

void array_list_add_to_end(struct array_list *list, char *str);

void destroy_array_list(struct array_list *list);
