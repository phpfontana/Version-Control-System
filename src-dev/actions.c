# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "data_structures.h"
# include "validations.h"
# include "files.h"
# include "actions.h"
# include "parser.h"

void vcs_init() {
    // Check if .vcs directory exists
    if (directory_exists(VCS_DIRECTORY) == 1) {
        printf("vcs: error: .vcs directory already exists\n");
        exit(EXIT_FAILURE);
    } 

    // Create .vcs directory
    if (create_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: could not create .vcs directory\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/commits.txt file
    if (write_empty_file(COMMITS_FILE) == 0) {
        printf("vcs: error: could not create .vcs/commits.txt file\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/stage.txt file
    if (write_empty_file(STAGE_FILE) == 0) {
        printf("vcs: error: could not create .vcs/stage.txt file\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/contents.txt file
    if (write_empty_file(CONTENTS_FILE) == 0) {
        printf("vcs: error: could not create .vcs/contents.txt file\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/metadata.txt file
    if (write_empty_file(METADATA_FILE) == 0) {
        printf("vcs: error: could not create .vcs/metadata.txt file\n");
        exit(EXIT_FAILURE);
    }

    printf("vcs: initialized empty vcs repository in %s\n", VCS_DIRECTORY);
}

void vcs_add(const char *path) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }

    // verifies if path was provided
    if (path == NULL) {
        printf("vcs: error: no path provided\n");
        exit(EXIT_FAILURE);
    }

    // verifies if path exists
    if (file_exists(path) == 0) {
        printf("vcs: error: %s file not found\n", path);
        exit(EXIT_FAILURE);
    }

    // verifies if path was already added
    if (path_is_staged(path) == 1) {
        printf("vcs: error: %s was already added to stage\n", path);
        exit(EXIT_FAILURE);
    }

    // append path to .vcs/stage.txt
    if (append_to_file(STAGE_FILE, path) == 0) {
        printf("vcs: error: could not append to .vcs/stage.txt\n");
        exit(EXIT_FAILURE);
    }

    // print success message
    printf("vcs: %s added to stage\n", path);
}

void vcs_commit(const char *message) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }

    // verifies if message was provided
    if (message == NULL) {
        printf("vcs: error: no message provided\n");
        exit(EXIT_FAILURE);
    }

    // verifies if stage is empty
    if (file_is_empty(STAGE_FILE) == 1) {
        printf("vcs: error: stage is empty\n");
        exit(EXIT_FAILURE);
    }

    // initialize commit head and files head
    CommitHead *commit_head = commit_create();
    FileHead *file_head = file_create();

    // read stage file
    parse_stage(file_head);

    // parse commit file
    parse_commit_file(commit_head, file_head, message);

    // display structs 
    commit_display(commit_head);

    file_display(file_head);

    // rewrite stage file
    write_empty_file(STAGE_FILE);

    // free memory
    commit_destroy(commit_head);
    file_destroy(file_head);

    // print success message
    printf("vcs: commit successful\n");
}

void vcs_log(const char *hash) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }

    // initialize commit head and files head
    CommitHead *commit_head = commit_create();

    parse_commits(commit_head);

    display_commits(commit_head);
}

void vcs_checkout(const char *hash) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
}
void vcs_show(const char *hash) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    vcs_log(NULL);
    return 0;
}

