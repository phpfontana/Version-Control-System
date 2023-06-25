# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "validations.h"
# include "files.h"
# include "utils.h"
# include "data_structures.h"
# include "actions.h"
# include "parse.h"

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

    // Parse file to stage
    parse_file_paths_to_stage(file_path);

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

    // Parse stage to Files
    parse_stage_to_files(commit);

    // Parse Files to contents file
    parse_files_to_contents(commit->files);

    // Parse Commit to commits file
    parse_commit_to_commit_file(commit);

    // Parse Commit to metadata file
    parse_commit_to_metadata_file(commit);

    // Rewrite stage file
    write_file(STAGE_FILE);

    // Free commit
    free_commit(commit);

    // Print Success message
    printf("vcs: commit successful\n");
}

void vcs_log()
{
    // Initialize the head of the linked list
    Commit* head = NULL;

    // Read the commits.txt file and populate the linked list
    parse_commit_file_to_commit(&head);

    // Print the commits from last to first
    print_commits(head);

    // Free the memory allocated for the linked list
    free_commits(head);
}

void vcs_checkout(const char *hash) {

    int byte_start, byte_end;

    // Parse metadata file to commit
    parse_metadata_file_to_commit(hash, byte_start, byte_end);

    // Print the commit ID
    printf("Commit ID: %s\n", hash);
     
    // Parse commit file to files
    parse_commit_to_files(byte_start, byte_end);
}


int main(int argc, char const *argv[])
{   
    vcs_checkout("CBB14230B7417704C21D816F19F3D797CA5F6463");
    return 0;
}

