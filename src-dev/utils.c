/**
 * @file main.c
 * @brief Example code demonstrating file operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "data_structures.h"
#include "utils.h"

/**
 * @brief Creates a directory.
 *
 * This function creates a new directory at the specified path.
 *
 * @param path The path of the directory to create.
 * @return 1 if the directory was successfully created, 0 otherwise.
 */
int create_directory(const char *path) {
    return (mkdir(path) == 0);
}

/**
 * @brief Writes content to a file.
 *
 * This function creates a new file at the specified path and writes the given content to it.
 *
 * @param path The path of the file to create.
 * @param content The content to write to the file.
 * @return 1 if the file was successfully written, 0 otherwise.
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
 * @brief Appends content to a file.
 *
 * This function opens an existing file at the specified path and appends the given content to it.
 *
 * @param path The path of the file to append to.
 * @param content The content to append to the file.
 * @return 1 if the file was successfully appended, 0 otherwise.
 */
int append_file(const char *path, const char *content) {
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        printf("vcs: error: could not open %s file\n", path);
        return 0;
    }
    fprintf(file, "%s\n", content);
    fclose(file);
    return 1;
}

/**
 * @brief Reads the contents of a file within a specified range.
 *
 * This function reads the contents of a file located at the specified path,
 * starting from the specified start_byte and ending at the specified end_byte.
 *
 * @param path The path of the file to read.
 * @param start_byte The starting byte position to read from.
 * @param end_byte The ending byte position to read until.
 * @return A dynamically allocated string containing the file contents, or NULL if an error occurred.
 */
char *read_file_contents(const char *path, int start_byte, int end_byte) {
    FILE *file = fopen(path, "r"); // Open file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open %s file\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END); // Seek to end of file
    int file_size = ftell(file); // Get current file pointer
    fseek(file, 0, SEEK_SET); // Seek back to beginning of file
    if (start_byte < 0) {
        start_byte = 0;
    }
    if (end_byte > file_size) {
        end_byte = file_size;
    }
    int content_size = end_byte - start_byte;  // Calculate content size
    char *content = malloc(content_size + 1);
    if (content == NULL) {
        printf("vcs: error: could not allocate memory\n");
        return NULL;
    }
    fseek(file, start_byte, SEEK_SET); // Seek to start byte
    fread(content, 1, content_size, file); // Read content
    content[content_size] = '\0';  // Add null terminator
    fclose(file);  // Close file
    return content; // Return content
}


/**
 * @brief Reads the contents of a file.
 *
 * This function reads the contents of a file located at the specified path.
 *
 * @param path The path of the file to read.
 * @return A dynamically allocated string containing the file contents, or NULL if an error occurred.
 */
char *read_file(const char *path) {
    FILE *file = fopen(path, "r"); // Open file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open %s file\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END); // Seek to end of file
    int file_size = ftell(file); // Get current file pointer
    fseek(file, 0, SEEK_SET); // Seek back to beginning of file
    char *content = malloc(file_size + 1);
    if (content == NULL) {
        printf("vcs: error: could not allocate memory\n");
        return NULL;  
    }
    fread(content, 1, file_size, file); // Read content
    content[file_size] = '\0';  // Add null terminator
    fclose(file);  // Close file
    return content; // Return content
}

/**
 * @brief Checks if a directory exists.
 *
 * This function checks if a directory exists at the specified path.
 *
 * @param path The path of the directory to check.
 * @return 1 if the directory exists, 0 otherwise.
 */
int directory_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

/**
 * @brief Checks if a file exists.
 *
 * This function checks if a file exists at the specified path.
 *
 * @param path The path of the file to check.
 * @return 1 if the file exists, 0 otherwise.
 */
int file_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

/**
 * @brief Validates the VCS directory and required files.
 *
 * This function validates the existence of the VCS directory and required files.
 *
 * @param path The path of the VCS directory.
 * @return 1 if the directory and files are valid, 0 otherwise.
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

    if (directory_exists(CONTENTS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs/contents directory not found\n");
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
 * This function checks if a file at the specified path is empty.
 *
 * @param path The path of the file to check.
 * @return 1 if the file is empty, 0 otherwise.
 */
int file_is_empty(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("vcs: error: %s file not found\n", path);
        return 0;
    }
    
    // seek to end of file
    fseek(file, 0, SEEK_END);

    // get size of file
    int size = ftell(file);

    // close file
    fclose(file);

    // return true if file is empty
    return (size == 0);
}

/**
 * @brief Checks if a path is staged.
 *
 * This function checks if a file or directory at the specified path is staged.
 *
 * @param path The path to check.
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

/**
 * @brief Generates a random hash.
 *
 * This function generates a random hash string.
 *
 * @return A dynamically allocated string containing the generated hash.
 */
char *hash() {
    char *hash = malloc(41 * sizeof(char));
    if (hash == NULL) {
        printf("Error allocating memory for hash\n");
        exit(EXIT_FAILURE);
    }

    const char *hexChars = "0123456789ABCDEF";
    srand(time(NULL));

    for (int i = 0; i < 40; i++) {
        hash[i] = hexChars[rand() % 16];
    }
    hash[40] = '\0';

    return hash;
}

/**
 * @brief Concatenates two strings.
 *
 * This function concatenates two strings and returns the result.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @return A dynamically allocated string containing the concatenated result.
 */
char *concat_strings(const char *str1, const char *str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // Allocate memory for the concatenated string
    char *result = (char *)malloc(len1 + len2 + 2); // +2 for the underscores and null terminator
    if (result == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the first string to the result
    strcpy(result, str1);

    // Append the second string to the result
    strcat(result, str2);

    return result;
}

/**
 * @brief Gets the current timestamp.
 *
 * This function gets the current timestamp as a string.
 *
 * @return A dynamically allocated string containing the current timestamp.
 */
char *timestamp() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}
