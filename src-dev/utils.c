# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"
# include "utils.h"

/// FILE OPERATIONS
int create_directory(const char *path) {
    return (mkdir(path, 0777) == 0);
}

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
int append_file(const char *path, const char *content) {
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        printf("vcs: error: could not open %s file\n", path);
        return 0;
    }
    fprintf(file, "%s", content);
    fclose(file);
    return 1;
}

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

/// VALIDATIONS
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
    
    // seek to end of file
    fseek(file, 0, SEEK_END);

    // get size of file
    int size = ftell(file);

    // close file
    fclose(file);

    // return true if file is empty
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
