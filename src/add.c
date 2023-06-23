# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>
# include <sys/stat.h>
# include <time.h>
# include "add.h"
# include "vcs.h"
# include "init.h"

int main(int argc, char const *argv[])
{
    // add file to staging area
    vcs_add("test.txt");
    return 0;
}

// verifies if directory exists
int directory_exists(char *path) {
    int status = access(path, 0);
    if (status == 0)
        return 1;  // directory exists
    else
        return 0;  // directory does not exist
}

// verify if file exists
int file_exists(char *path) {
    int status = access(path, 0);
    if (status == 0)
        return 1;  // file exists
    else
        return 0;  // file does not exist
}

// Verify if path was already added to file
int path_already_added(const char* file_path, const char* output_file) {
    FILE* file = fopen(output_file, "r");  // Opens file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", output_file);
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {  // Reads file line by line
        line[strcspn(line, "\n")] = '\0';  // Remove the trailing newline character

        if (strcmp(line, file_path) == 0) {  // Compares file path with line
            fclose(file);
            free(line);
            return 1;  // Path was already added
        }
    }

    fclose(file);
    free(line);
    return 0;  // Path was not added
}

// add paths to files
void add_path_to_file(char* file_path, char* output_file) {
    FILE* file = fopen(output_file, "a");  // opens file in append mode
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", output_file);
        exit(EXIT_FAILURE);
    }

    char* file_path_copy = strdup(file_path);  // creates copy of file paths
    if (file_path_copy == NULL) {
        printf("vcs: error: could not allocate memory for file paths\n");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(file_path_copy, ", ");  // gets first file path
    while (token != NULL) {
        if (path_already_added(token, output_file) == 1) {  // verifies if path was already added
            printf("vcs: error: file %s was already added\n", token);
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s\n", token);  // writes file path to file
        token = strtok(NULL, ", ");  // gets next file path
    }
    fclose(file);
    free(file_path_copy);
}

// appends file or files to staging area
void vcs_add(char* file_path) {

    // verifies if .vcs folder exists
    if (directory_exists(VCS_FOLDER) == 0) {
        printf("vcs: error: .vcs folder does not exist\n");
        exit(EXIT_FAILURE);
    }
    
    // verifies if staging area exists
    if (directory_exists(STAGE_FILE) == 0) {
        printf("vcs: error: staging area does not exist\n");
        exit(EXIT_FAILURE);
    }

    // verifies if contents.txt exists
    if (file_exists(CONTENTS_FILE) == 0) {
        printf("vcs: error: file or files do not exist\n");
        exit(EXIT_FAILURE);
    }
    
    // verifies if file or files were specified
    if (file_path == NULL) {
        printf("vcs: error: no file or files were specified\n");
        exit(EXIT_FAILURE);
    }

    // add file or files to staging area
    add_path_to_file(file_path, STAGE_FILE);
}
