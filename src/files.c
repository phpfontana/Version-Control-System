# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"
# include "validations.h"
# include "actions.h"
# include "files.h"

int create_directory(const char *path) {
    return (mkdir(path, 0777) == 0);
}

int write_empty_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create %s file\n", path);
        return 0;
    }
    fclose(file);
    return 1;
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

FILE *open_file(const char *path, const char *mode) {
    FILE *file = fopen(path, mode);
    if (file == NULL) {
        printf("vcs: error: could not open %s file\n", path);
        return NULL;
    }
    return file;
}

int append_to_file(const char *path, const char *content) {
    FILE *file = open_file(path, "a");
    if (file == NULL) {
        return 0;
    }
    fprintf(file, "%s\n", content);
    fclose(file);
    return 1;
}

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
