# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "validations.h"

// Verifies if directory exists
int directory_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

// verify if file exists
int file_exists(const char* path)
{
    int status = access(path, F_OK);
    return status == 0;
}

// validates directory
int validate_directory(void) {
    // verifies if .vcs folder exists
    if (directory_exists(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs folder does not exist\n");
        return 0;
    }

    // verifies if database files exist
    if (file_exists(CONTENTS_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", CONTENTS_FILE);
        return 0;
    }

    if (file_exists(COMMITS_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", COMMITS_FILE);
        return 0;
    }

    if (file_exists(METADATA_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", METADATA_FILE);
        return 0;
    }

    if (file_exists(STAGE_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", STAGE_FILE);
        return 0;
    }

    return 1;
}

// Verify if file is empty
int file_is_empty(const char *path) {
    FILE *file = fopen(path, "r");  // open file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open file\n");
        exit(EXIT_FAILURE);
    }

    // get file size
    fseek(file, 0, SEEK_END);  // set pointer to end of file
    int size = ftell(file);  // get file size
    
    // close file
    fclose(file);

    // return 1 if file is empty, 0 otherwise 
    if (size == 0)
        return 1;  
    else
        return 0;
}

