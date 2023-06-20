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
    vcs_add("vcs.h, vcs.c");
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


// process file
void process_file(const char* file_path) {

    FILE* file = fopen(file_path, "rb");  // opens file in binary mode
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    // gets file size
    fseek(file, 0, SEEK_END);  // sets file pointer to end of file
    long file_size = ftell(file);  // gets file size
    fseek(file, 0, SEEK_SET);  // sets file pointer to beginning of file

    // reads file content
    unsigned char* buffer = (unsigned char*) malloc(file_size * sizeof(unsigned char));
    if (buffer == NULL) {
        printf("vcs: error: could not allocate memory for file %s\n", file_path);
        exit(EXIT_FAILURE);
    }
    fread(buffer, sizeof(unsigned char), file_size, file);  // reads file content
    fclose(file);

    // store contents in contents.txt
    FILE* contents_file = fopen(CONTENTS_FILE, "ab");  // opens file in binary mode
    if (contents_file == NULL) {
        printf("vcs: error: could not open file %s\n", CONTENTS_FILE);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    long start_byte_position = ftell(contents_file);  // gets start byte position
    fwrite(buffer, sizeof(unsigned char), file_size, contents_file);  // writes file content
    long end_byte_position = ftell(contents_file) - 1;  // gets end byte position
    
    fclose(contents_file);

    // store start and end byte positions in stage.txt
    FILE* stage_file = fopen(STAGE_FILE, "a");  // opens file in append mode
    if (stage_file == NULL) {
        printf("vcs: error: could not open file %s\n", STAGE_FILE);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // writes file path, start byte position and end byte position to stage.txt
    fprintf(stage_file, "%s %ld %ld\n", file_path, start_byte_position, end_byte_position);
    fclose(stage_file);

    free(buffer);
}

// separate file paths
void stage_files(char* file_paths) {
    char* file_path_copy = strdup(file_paths);  // creates copy of file paths
    if (file_path_copy == NULL) {
        printf("vcs: error: could not allocate memory for file paths\n");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(file_path_copy, ", ");  // gets first file path
    while (token != NULL) {
        process_file(token);  // processes file
        token = strtok(NULL, ", ");  // gets next file path
    }

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

    // stage files to stage.txt and contents to contents.txt
    stage_files(file_path);

}





