/**
 * @file validations.c
 * @brief Validation functions for the VCS (Version Control System).
 *
 * This file contains validation functions used in the VCS system.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "data_structures.h"
# include "validations.h"

/**
 * @brief Checks if a directory exists.
 *
 * @param path The path to the directory.
 * @return 1 if the directory exists, 0 otherwise.
 */
int directory_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

/**
 * @brief Checks if a file exists.
 *
 * @param path The path to the file.
 * @return 1 if the file exists, 0 otherwise.
 */
int file_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

/**
 * @brief Validates the VCS directory and required files.
 *
 * @param path The path to the VCS directory.
 * @return 1 if the directory and required files are valid, 0 otherwise.
 */
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

/**
 * @brief Checks if a file is empty.
 *
 * @param path The path to the file.
 * @return 1 if the file is empty, 0 otherwise.
 */
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

/**
 * @brief Checks if a path is staged.
 *
 * @param path The path to the file.
 * @return 1 if the path is staged, 0 otherwise.
 */
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