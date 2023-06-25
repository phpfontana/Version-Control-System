# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"
# include "validations.h"
# include "files.h"

// New Commit
Commit *new_commit(const char* hash, const char* message, const char* date) {
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory for commit\n");
        exit(EXIT_FAILURE);
    }
    if (hash != NULL) {
        commit->hash = (char *) malloc(sizeof(char) * (strlen(hash) + 1));
        strcpy(commit->hash, hash);
    }
    else {
        commit->hash = NULL;
    }
    if (message != NULL) {
        commit->message = (char *) malloc(sizeof(char) * (strlen(message) + 1));
        strcpy(commit->message, message);
    }
    else {
        commit->message = NULL;
    }
    if (date != NULL) {
        commit->date = (char *) malloc(sizeof(char) * (strlen(date) + 1));
        strcpy(commit->date, date);
    }
    else {
        commit->date = NULL;
    }
    commit->byte_start = 0;
    commit->byte_end = 0;
    commit->files = NULL;
    commit->next = NULL;
    commit->prev = NULL;

    return commit;
}

// New File
Files *new_file(const char *file_path) {
    Files *file = (Files *) malloc(sizeof(Files));
    if (file == NULL) {
        printf("vcs: error: could not allocate memory for file\n");
        exit(EXIT_FAILURE);
    }
    if (file_path != NULL) {
        file->file_path = (char *) malloc(sizeof(char) * (strlen(file_path) + 1));
        strcpy(file->file_path, file_path);
    }
    else {
        file->file_path = NULL;
    }
    file->byte_start = 0;
    file->byte_end = 0;
    file->next = NULL;
    file->prev = NULL;

    return file;
}

// Insert File into Commit
void insert_file (Commit* commit, Files* file) {
    Files *current = commit->files;
    if (current == NULL) {
        commit->files = file;
        return;
    }
    else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = file;
        file->prev = current;
    }
}

// free commit
void free_commit(Commit* commit){
    if (commit == NULL) {
        return;
    }

    // Free the hash and date strings
    free(commit->hash);
    free(commit->date);

    // Free the files linked list
    Files* file = commit->files;
    while (file != NULL) {
        Files* next = file->next;
        free(file->file_path);
        free(file);
        file = next;
    }

    // Free the message string
    free(commit->message);

    // Recursive call to free the previous commit
    free_commit(commit->prev);

    // Free the commit itself
    free(commit);
}

// free commits
void free_commits(Commit* head) {
    Commit* current = head;
    while (current != NULL) {
        Commit* next = current->next;
        free(current->hash);
        free(current->date);
        free(current->message);
        free(current);
        current = next;
    }
}

void print_commits(Commit* commit) {
    Commit* current = commit;
    while (current != NULL) {
        printf("Commit: %s\n", current->hash);
        printf("Date: %s\n", current->date);
        printf("Message: %s\n", current->message);
        printf("\n");

        current = current->next;
    }
}








