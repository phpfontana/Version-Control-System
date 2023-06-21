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
# include "commit.h"


int main(int argc, char const *argv[])
{
    vcs_commit("testing commit");
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

// verifies if file is empty
int is_empty(char *path) {
    FILE *file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);
    if (size == 0)
        return 1;  // file is empty
    else
        return 0;  // file is not empty
}

// verify if file exists
int file_exists(char *path) {
    int status = access(path, 0);
    if (status == 0)
        return 1;  // file exists
    else
        return 0;  // file does not exist
}

// generates a random hash
char *get_hash() {
    char *hash = malloc(41 * sizeof(char));
    for (int i = 0; i < 40; i++) {
        hash[i] = (rand() % 16) + '0';
    }
    hash[40] = '\0';
    return hash;
}

// generate timestamp
char *get_timestamp() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}

void vcs_commit(char *message) 
{
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

    // verifies if staging area is empty
    if (is_empty(STAGE_FILE) == 1) {
        printf("vcs: error: staging area is empty\n");
        exit(EXIT_FAILURE);
    }

    // verifies if commits file exists
    if (file_exists(COMMITS_FILE) == 0) {
        printf("vcs: error: commits file does not exist\n");
        exit(EXIT_FAILURE);
    }

    // initializes Commit struct
    Commit* commit = (Commit*) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory for commit\n");
        exit(EXIT_FAILURE);
    }
    commit->hash = get_hash();
    commit->message = message;
    commit->date = get_timestamp();
    commit->byte_start = 0;
    commit->byte_end = 0;
    commit->files = NULL;
    commit->next = NULL;
    commit->prev = NULL;

    // initialize Files struct
    Files* new_file = (Files*) malloc(sizeof(Files));
    if (new_file == NULL) {
        printf("vcs: error: could not allocate memory for file\n");
        exit(EXIT_FAILURE);
    }
    commit->files = new_file;
    commit->files->file_path = NULL;
    commit->files->byte_start = 0;
    commit->files->byte_end = 0;
    commit->files->next = NULL;
    commit->files->prev = NULL;

    // print Commit struct data to commits file
    FILE *commits_file = fopen(COMMITS_FILE, "a");
    if (commits_file == NULL) {
        printf("vcs: error: could not open commits file\n");
        exit(EXIT_FAILURE);
    }
    commit->byte_start = ftell(commits_file);
    fprintf(commits_file, "ID: %s\n", commit->hash);
    fprintf(commits_file, "Date: %s\n", commit->date);
    // append staged files to commit
    FILE *stage_file = fopen(STAGE_FILE, "r");
    if (stage_file == NULL) {
        printf("vcs: error: could not open stage file\n");
        exit(EXIT_FAILURE);
    }
    int ch;
    while ((ch = fgetc(stage_file)) != EOF) {
        fputc(ch, commits_file);
    }
    fclose(stage_file);
    fprintf(commits_file, "Message: %s\n", commit->message);
    
    // get byte start and end of commit 
    // check if metadata.txt is empty
    if (is_empty(METADATA_FILE) == 1) {
        commit->byte_start = 0;
        commit->byte_end = ftell(commits_file);
    }
        
    fseek(commits_file, 0, SEEK_END);
    commit->byte_end = ftell(commits_file) - 1;
    fclose(commits_file);
    
    // save to metadata.txt
    FILE *metadata_file = fopen(METADATA_FILE, "a");
    if (metadata_file == NULL) {
        printf("vcs: error: could not open metadata file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(metadata_file, "%d %d\n", commit->byte_start, commit->byte_end);

    fclose(metadata_file);

    // rewrite stage file
    stage_file = fopen(STAGE_FILE, "w");
    if (stage_file == NULL) {
        printf("vcs: error: could not open stage file\n");
        exit(EXIT_FAILURE);
    }
    fclose(stage_file);

    // print success message
    printf("vcs: commit: %s\n", commit->hash);
    printf("vcs: message: %s\n", commit->message);
    printf("vcs: date: %s\n", commit->date);
    printf("vcs: files: ");
    Files *file = commit->files;
    while (file != NULL) {
        printf("%s ", file->file_path);
        file = file->next;
    }
    printf("\n");

    // free memory
    free(commit);
    free(new_file);

    exit(EXIT_SUCCESS);
}

// TODO
// Modularize
// fix metadata.txt