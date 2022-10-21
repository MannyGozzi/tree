#define DIR_NAME_POS 1

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> // allows us to use S_ISDIR()
#include "array_list.h"
#define TYPE_DIR 100
#define TYPE_FILE 101

void validate_params(int argc, char *argv[])
{
    if (argc < 2 || argv[DIR_NAME_POS] == NULL)
    {
        perror("Validate params");
        exit(1); 
    }
}

// qsort requires parameters to be taken in as void *, we have to cast to char *
int cmpstr_void(const void* voida, const void* voidb)
{
    const char* a = *(const char**)voida;
    const char* b = *(const char**)voidb;
    return strcmp(a,b);
}

void get_sorted_dir_entries(char* dir_name, struct array_list* entries) {
    DIR* dir = opendir(dir_name);
    chdir(dir_name); // we have to move into the directory to read what's inside
    struct dirent* entry = readdir(dir);

    while (entry != NULL) {
        array_list_add_to_end(entries, entry->d_name);
        entry = readdir(dir);
    }
    qsort(entries->content, entries->size, sizeof(char*), cmpstr_void);
    closedir(dir);
}

int get_entry_type(char* entry_name) {
    struct stat stat_buf;
    if(stat(entry_name, &stat_buf) == -1) {
        perror("lstat error");
        exit(1);
    } else if(S_ISDIR(stat_buf.st_mode)) {
        // when we're in a directory, call recursively
        return TYPE_DIR;
    } else {
        // when we have a file
        return TYPE_FILE;
    }
}

void print_item_with_formatting(char* item_name, int level, int type) {
    if (type == TYPE_DIR) {
        for (int i = 0; i < level; ++i) printf("|-- ");
    } else {
        for (int i = 0; i < level; ++i) printf("|   ");
        printf("|-- ");
    }

    printf("%s", item_name);
    // print size if requested
    printf("\n");
}

// get all the names inside the directory
// use qsort to sort all the names
// print recursively when a new directory comes up

// prints the contents of a directory recursively (just not yet)
void print_directory_r(char* dir_name, int level) {
    struct array_list *entries = new_array_list();
    get_sorted_dir_entries(dir_name, entries);
    print_item_with_formatting(dir_name, level, TYPE_DIR);
    for (int i = 0; i < entries->size; ++i) {
        int type = get_entry_type(entries->content[i]);
        if (strcmp(entries->content[i], "..") == 0 || strcmp(entries->content[i], ".") == 0) continue;
        if (type == TYPE_DIR) {
            // print recursively
            print_directory_r(entries->content[i], ++level);
            --level;
            chdir(".."); // if we go into this dir we need to exit if there are files following it
        } else {
            print_item_with_formatting(entries->content[i], level, TYPE_FILE);
        }
    }
}

int main(int argc, char *argv[])
{
    validate_params(argc, argv);
    print_directory_r(argv[DIR_NAME_POS], 0);
    return 0;
}
