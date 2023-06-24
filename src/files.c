# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "files.h"

// Creates a directory
int create_directory(const char *path) {
    int status = mkdir(path, 0777);
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

// Verify if path was already added to file
int path_already_added(const char* file_path, const char* output_file) {
    FILE* file = open_file(output_file, "r");  // Opens file in read mode
    
    // Reads file line by line
    char* line = NULL;  // Stores the line
    size_t len = 0;  // Stores the length of the line
    ssize_t read;  // Stores the number of characters read
    
    while ((read = getline(&line, &len, file)) != -1) {  // Reads file line by line
        line[strcspn(line, "\n")] = '\0';  // Remove the trailing newline character

        if (strcmp(line, file_path) == 0) {  // Compares file path with line
            fclose(file);
            free(line);
            return 1;  // Path was already added
        }
    }

    close_file(file);
    free(line);
    return 0;  // Path was not added
}
