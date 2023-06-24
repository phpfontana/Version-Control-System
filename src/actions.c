# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "validations.h"
# include "files.h"
# include "utils.h"
# include "data_structures.h"
# include "actions.h"

void vcs_init() {
    // Verifies if .vcs directory exists
    if (directory_exists(VCS_DIRECTORY)) {
        printf("vcs: error: .vcs directory already exists\n");
        exit(EXIT_FAILURE);
    }
    
    // Creates .vcs directory
    if (!create_directory(VCS_DIRECTORY)) {
        printf("vcs: error: could not create .vcs directory\n");
        exit(EXIT_FAILURE);
    }
    
    // Writes necessary files
    write_file(COMMITS_FILE);
    write_file(STAGE_FILE);
    write_file(CONTENTS_FILE);
    write_file(METADATA_FILE);
    
    // Prints success message
    printf("vcs: initialized empty version control system repository in .vcs\n");
}

void vcs_add(const char* file_path) {

    // validates directory
    if (validate_directory() == 0) {
        printf("vcs: error: .vcs was not initialized properly\n");
        exit(EXIT_FAILURE);
    }
    
    // verifies if file or files were specified
    if (file_path == NULL) {
        printf("vcs: error: no file or files were specified\n");
        exit(EXIT_FAILURE);
    }

    // Open Stage file
    FILE *stage_file = open_file(STAGE_FILE, "a");

    // make copy of file paths
    char *file_paths = strdup(file_path);

    // get first file path
    char *token = strtok(file_paths, ", ");
    if (token == NULL) {
        printf("vcs: error: no file or files were specified\n");
        exit(EXIT_FAILURE);
    }

    // loop through file paths
    while (token != NULL) {
        // verify if file exists
        if (file_exists(token) == 0) {
            printf("vcs: error: file %s does not exist\n", token);
            exit(EXIT_FAILURE);
        
        // verify if file path was already added
        } else if (path_already_added(token, STAGE_FILE) == 1) {
            printf("vcs: error: file %s was already added\n", token);
            exit(EXIT_FAILURE);
        }

        // write file path to stage file
        fprintf(stage_file, "%s\n", token);

        // get next file path
        token = strtok(NULL, ", ");
    }

    // close stage file
    close_file(stage_file);

    // free memory
    free(file_paths);

    // success message
    printf("vcs: file paths added to staging area\n");
}

void vcs_commit(const char *message) {
    // validates directory
    if (validate_directory() == 0)
    {
        printf("vcs: error: .vcs was not initialized properly\n");
        exit(EXIT_FAILURE);
    }

    // verifies if staging area is empty
    if (file_is_empty(STAGE_FILE) == 1) {
        printf("vcs: error: staging area is empty\n");
        exit(EXIT_FAILURE);
    }

    // verifies if commit message is empty
    if (message == NULL) {
        printf("vcs: error: commit message is empty\n");
        exit(EXIT_FAILURE);
    }

    // initializes commit
    Commit *commit = new_commit(get_hash() ,message, get_timestamp());

    // Open stage file in read mode
    FILE *stage_file = open_file(STAGE_FILE, "r");

    // buffer for reading file paths
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stage_file)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character
        Files* file = new_file(buffer);  // Create new file
        insert_file(commit, file);  // Insert file into commit
    }

    // close stage file
    close_file(stage_file);

    // Open contents file in append mode
    FILE *contents_file = open_file(CONTENTS_FILE, "a");

    Files *current = commit->files;
    current->byte_start = ftell(contents_file);

    // loop through files
    while (current != NULL) {
        // open file from Files in read mode
        FILE *file = open_file(current->file_path, "r");
        if (file == NULL) {
            printf("vcs: error: could not open file %s\n", current->file_path);
            exit(EXIT_FAILURE);
        }
        
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            fputs(buffer, contents_file);
        }
        fputs("\n", contents_file);

        // store byte end
        current->byte_end = ftell(contents_file) - 1;

        // go to next file
        current = current->next;

        // add newline character if there is a next file
        if (current != NULL) {
            fputs("\n", contents_file);
        }
    }

    // close contents file
    close_file(contents_file);

    // Open commits file in append mode
    FILE *commits_file = open_file(COMMITS_FILE, "a");

    // store byte start
    commit->byte_start = ftell(commits_file);

    // Write commit to file
    fprintf(commits_file, "ID: %s\n", commit->hash);
    fprintf(commits_file, "Date: %s\n", commit->date);
    fprintf(commits_file, "Files:\n");
    Files *current2 = commit->files;
    while (current2 != NULL) {
        fprintf(commits_file, "%s %d %d\n", current2->file_path, current2->byte_start, current2->byte_end);
        current2 = current2->next;
    }
    fprintf(commits_file, "Message: %s\n", commit->message);

    // Write commit separator
    fprintf(commits_file, "---------------------\n");
    
    // store byte end
    commit->byte_end = ftell(commits_file) - 1;

    // close commits file
    close_file(commits_file);

    // append commit to metadata.txt
    FILE *metadata_file = open_file(METADATA_FILE, "a");
    fprintf(metadata_file, "%s %d %d\n", commit->hash, commit->byte_start, commit->byte_end);
    close_file(metadata_file);

    // rewrite stage file
    write_file(STAGE_FILE);

    // free memory
    free(commit);

    // success message
    printf("vcs: commit successful\n");
}

int main(int argc, char const *argv[])
{
    vcs_commit("testing commit");
    return 0;
}

