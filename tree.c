#define DIR_NAME_POS 1

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> // allows us to use S_ISDIR()

void validate_params(int argc, char *argv[])
{
    if (argc < 2 || argv[DIR_NAME_POS] == NULL)
    {
        perror("Validate params");
        exit(1); 
    }
}

// prints the contents of a directory recursively (just not yet)
void print_directory_r(char* dir_name) {
    DIR* dir = opendir(dir_name);
    chdir(dir_name); // we have to move into the directory to read what's inside
    struct dirent* entry = readdir(dir);
    struct stat stat_buf;
    while (entry != NULL) {
        if(stat(entry->d_name, &stat_buf) == -1) {
            perror("lstat error");
            printf("file/dir  that is listed as an error: %s\n", entry->d_name);
            exit(1);
        }
        if(S_ISDIR(stat_buf.st_mode)) {
            // when we're in a directory, call recursively
            printf("%s is a dir\n", entry->d_name);
        } else {
            // when we have a file
            printf("%s\n", entry->d_name);
        }
        entry = readdir(dir);
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    validate_params(argc, argv);
    print_directory_r(argv[DIR_NAME_POS]);
    return 0;
}
