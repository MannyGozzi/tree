#define DIR_NAME_POS 1

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> // allows us to use S_ISDIR()
#include "array_list.h"
#include <stdbool.h>
#define TYPE_DIR 100
#define TYPE_FILE 101

void get_param_flags(int *dir_name_pos, bool *show_hidden, bool *show_size, int argc, char *argv[])
{
    // the first name should be a directory name
    for (int i = DIR_NAME_POS; i < argc; ++i) {
        char* tok = argv[i];
        if (strcmp(tok, "-a") == 0) {
            *show_hidden = true;
        }
        else if (strcmp(tok, "-s") == 0) {
            *show_size = true;
        } else if(tok != NULL) {
            *dir_name_pos = i;
            break;
        }
    }
}

// qsort requires parameters to be taken in as void *, we have to cast to char *
int cmpstr_void(const void *voida, const void *voidb)
{
    const char *a = *(const char **)voida;
    const char *b = *(const char **)voidb;
    return strcmp(a, b);
}

void get_sorted_dir_entries(char *dir_name, struct array_list *entries)
{
    DIR *dir = opendir(dir_name);
    chdir(dir_name); // we have to move into the directory to read what's inside
    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        array_list_add_to_end(entries, entry->d_name);
        entry = readdir(dir);
    }
    qsort(entries->content, entries->size, sizeof(char *), cmpstr_void);
    closedir(dir);
}

int get_entry_size(char *entry_name)
{
    struct stat stat_buf;
    if (stat(entry_name, &stat_buf) == -1)
    {
        perror("lstat error");
        exit(1);
    } 
    return (int) stat_buf.st_size;
}

int get_entry_type(char *entry_name)
{
    struct stat stat_buf;
    if (stat(entry_name, &stat_buf) == -1)
    {
        perror("lstat error");
        exit(1);
    }
    else if (S_ISDIR(stat_buf.st_mode))
    {
        // when we're in a directory, call recursively
        return TYPE_DIR;
    }
    else
    {
        // when we have a file
        return TYPE_FILE;
    }
}

bool is_hidden_file(char *name)
{
    if (name != NULL && name[0] != '.')
        return false;
    return true;
}

void print_item_with_formatting(char *item_name, int level, int type, bool show_hidden, bool show_size, int size)
{
    if (!show_hidden && is_hidden_file(item_name))
        return; // skip hidden files when appropriate
    if (type == TYPE_DIR)
    {
        for (int i = 0; i < level; ++i)
            printf("|-- ");
    }
    else
    {
        for (int i = 0; i < level; ++i)
            printf("|   ");
        printf("|-- ");
    }

    printf("%s", item_name);
    if (show_size) printf(" [size: %d]", size);
    // print size if requested
    printf("\n");
}

// get all the names inside the directory
// use qsort to sort all the names
// print recursively when a new directory comes up

// prints the contents of a directory recursively (just not yet)
void print_directory_r(char *dir_name, int level, bool show_hidden, bool show_size, int size)
{
    struct array_list *entries = new_array_list();
    get_sorted_dir_entries(dir_name, entries);
    print_item_with_formatting(dir_name, level, TYPE_DIR, show_hidden, show_size, size);
    for (int i = 0; i < entries->size; ++i)
    {
        int file_size = get_entry_size(entries->content[i]);
        int type = get_entry_type(entries->content[i]);
        if (strcmp(entries->content[i], "..") == 0 || strcmp(entries->content[i], ".") == 0) {
            //print_item_with_formatting(entries->content[i], level, TYPE_FILE, show_hidden, show_size, get_entry_size(entries->content[i]));
            continue;
        }
        if (type == TYPE_DIR)
        {
            // print recursively
            print_directory_r(entries->content[i], ++level, show_hidden, show_size, file_size);
            --level;
            chdir(".."); // if we go into this dir we need to exit if there are files following it
        }
        else
        {
            print_item_with_formatting(entries->content[i], level, TYPE_FILE, show_hidden, show_size, file_size);
        }
    }
    destroy_array_list(entries);
}

int main(int argc, char *argv[])
{
    int dir_name_pos = -1;
    char* dir_name = NULL;
    bool show_hidden = false, show_size = false;
    char start_dir[1024] = "";
    getcwd(start_dir, 1024);
    get_param_flags(&dir_name_pos, &show_hidden, &show_size, argc, argv);
    if (dir_name_pos != -1) dir_name = argv[dir_name_pos];
    else dir_name = start_dir;
    print_directory_r(dir_name, 0, show_hidden, show_size, get_entry_size(dir_name));    
    return 0;
}
