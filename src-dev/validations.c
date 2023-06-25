# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "data_structures.h"
# include "validations.h"

int directory_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

int file_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

int validate_directory(const char *path) {
    // Check if directory exists
    if (directory_exists(VCS_DIRECTORY) == 0 ) {
        printf("vcs: error: .vcs directory not found\n");
        return 0;
    }

    // Check if files exist
    if (file_exists(COMMITS_FILE) == 0) {
        printf("vcs: error: .vcs/commit file not found\n");
        return 0;
    }
    if (file_exists(STAGE_FILE) == 0) {
        printf("vcs: error: .vcs/stage file not found\n");
        return 0;
    }
    if (file_exists(CONTENTS_FILE) == 0) {
        printf("vcs: error: .vcs/contents file not found\n");
        return 0;
    }
    if (file_exists(METADATA_FILE) == 0) {
        printf("vcs: error: .vcs/metadata file not found\n");
        return 0;
    }
    
    return 1;
}

int file_is_empty(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("vcs: error: %s file not found\n", path);
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    int size = ftell(file);

    // close file
    fclose(file);

    return (size == 0);
}

int path_is_staged(const char *path) {
    FILE *file = fopen(STAGE_FILE, "r");
    if (file == NULL) {
        printf("vcs: error: .vcs/stage file not found\n");
        return 0;
    }

    // read file line by line
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove the trailing newline character
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(path, line) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}