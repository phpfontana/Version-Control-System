# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "validations.h"
# include "files.h"


// Creates a directory
int create_directory(const char *path) {
    int status = mkdir(path);
    return (status == 0);
}

// Creates a file
void write_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create file %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// Opens a file
FILE* open_file(const char* path, const char* mode) {
    FILE* file = fopen(path, mode);
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", path);
        exit(EXIT_FAILURE);
    }
    return file;
}

// Closes a file
void close_file(FILE* file) {
    fclose(file);
}

int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}
