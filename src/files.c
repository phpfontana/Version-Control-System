/**
 * @file files.c
 * @brief File-related functions for the VCS (Version Control System).
 *
 * This file contains functions for creating, writing, reading, and appending to files in the VCS system.
 */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"
# include "validations.h"
# include "actions.h"
# include "files.h"

/**
 * @brief Creates a directory at the specified path.
 *
 * @param path The path of the directory to create.
 * @return 1 if the directory was created successfully, 0 otherwise.
 */
int create_directory(const char *path) {
    return (mkdir(path, 0777) == 0);
}

/**
 * @brief Writes an empty file at the specified path.
 *
 * @param path The path of the file to create.
 * @return 1 if the file was created successfully, 0 otherwise.
 */
int write_empty_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create %s file\n", path);
        return 0;
    }
    fclose(file);
    return 1;
}

/**
 * @brief Writes content to a file at the specified path.
 *
 * @param path The path of the file to write.
 * @param content The content to write to the file.
 * @return 1 if the file was written successfully, 0 otherwise.
 */
int write_file(const char *path, const char *content) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create %s file\n", path);
        return 0;
    }
    fprintf(file, "%s", content);
    fclose(file);
    return 1;
}

/**
 * @brief Opens a file at the specified path with the specified mode.
 *
 * @param path The path of the file to open.
 * @param mode The mode in which to open the file (e.g., "r" for reading, "w" for writing).
 * @return A pointer to the opened file, or NULL if an error occurred.
 */
FILE *open_file(const char *path, const char *mode) {
    FILE *file = fopen(path, mode);
    if (file == NULL) {
        printf("vcs: error: could not open %s file\n", path);
        return NULL;
    }
    return file;
}

/**
 * @brief Appends content to a file at the specified path.
 *
 * @param path The path of the file to append to.
 * @param content The content to append to the file.
 * @return 1 if the content was appended successfully, 0 otherwise.
 */
int append_to_file(const char *path, const char *content) {
    FILE *file = open_file(path, "a");
    if (file == NULL) {
        return 0;
    }
    fprintf(file, "%s\n", content);
    fclose(file);
    return 1;
}

/**
 * @brief Reads the content of a file at the specified path.
 *
 * @param path The path of the file to read.
 * @return A dynamically allocated string containing the file content, or NULL if an error occurred.
 *         The caller is responsible for freeing the allocated memory.
 */
char *read_file(const char *path) {
    FILE *file = open_file(path, "r");
    if (file == NULL) {
        return NULL;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // allocate memory for file content
    char *content = malloc(file_size * sizeof(char));
    if (content == NULL) {
        printf("vcs: error: could not allocate memory for %s file\n", path);
        return NULL;
    }

    // read file content
    fread(content, sizeof(char), file_size, file);
    fclose(file);
    return content;
}
