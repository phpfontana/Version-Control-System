# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <time.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"
# include "utils.h"
# include "commands.h"

void vcs_init() {
    // Check if .vcs directory already exists
    if (directory_exists(VCS_DIRECTORY) == 1) {
        printf("vcs: error: .vcs directory already exists\n");
        return;
    }

        // Create .vcs directory
    if (create_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: could not create .vcs directory\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/commits.txt file
    if (write_file(COMMITS_FILE, EMPTY) == 0) {
        printf("vcs: error: could not create .vcs/commits.txt file\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/stage.txt file
    if (write_file(STAGE_FILE, EMPTY) == 0) {
        printf("vcs: error: could not create .vcs/stage.txt file\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/contents/ directory
    if (create_directory(CONTENTS_DIRECTORY) == 0) {
        printf("vcs: error: could not create .vcs/contents/ directory\n");
        exit(EXIT_FAILURE);
    }

    // Create .vcs/metadata.txt file
    if (write_file(METADATA_FILE, EMPTY) == 0) {
        printf("vcs: error: could not create .vcs/metadata.txt file\n");
        exit(EXIT_FAILURE);
    }

    printf("vcs: initialized empty vcs repository in %s\n", VCS_DIRECTORY);
}

void vcs_add(const char *path) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        return;
    }

    // verifies if path was provided
    if (path == NULL) {
        printf("vcs: error: no path provided\n");
        exit(EXIT_FAILURE);
    }

    // verifies if path exists
    if (file_exists(path) == 0) {
        printf("vcs: error: path does not exist\n");
        exit(EXIT_FAILURE);
    }

    // verifies if path was already added
    if (path_is_staged(path) == 1) {
        printf("vcs: error: path already added\n");
        exit(EXIT_FAILURE);
    }

    // append path to stage file
    if (append_file(STAGE_FILE, path) == 0) {
        printf("vcs: error: could not add path\n");
        exit(EXIT_FAILURE);
    }

    // print success message
    printf("vcs: added '%s'\n", path);
}

void vcs_commit(const char *message) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        return;
    }

    // verifies is message was provided
    if (message == NULL) {
        printf("vcs: error: no message provided\n");
        exit(EXIT_FAILURE);
    }

    // verifies if stage file is empty
    if (file_is_empty(STAGE_FILE) == 1) {
        printf("vcs: error: stage is empty\n");
        exit(EXIT_FAILURE);
    }

    // initialize FileHead and Commit Tree
    FileHead *file_head = file_create();
    CommitTree *commit_tree = commit_tree_create();

    // read stage file
    char *stage_content = read_file(STAGE_FILE);

    // split stage content by new line
    char *stage_path = strtok(stage_content, "\n");

    // iterate over stage paths
    while (stage_path != NULL) {
        // read file contents
        char *file_content = read_file(stage_path);

        // get file_id
        char *file_id = hash();

        // write empty file to contents directory
        char *file_path = concat_strings(CONTENTS_DIRECTORY, file_id);

        if (write_file(file_path, EMPTY) == 0) {
            printf("vcs: error: could not write file to contents directory\n");
            exit(EXIT_FAILURE);
        }
        
        // append file path to empty file
        if (append_file(file_path, stage_path) == 0) {
            printf("vcs: error: could not append file path to file\n");
            exit(EXIT_FAILURE);
        }

        // append file contents to file
        if (append_file(file_path, file_content) == 0) {
            printf("vcs: error: could not append file contents to file\n");
            exit(EXIT_FAILURE);
        }

        // insert file to commit
        file_insert(file_head, file_id);

        // free file content
        free(file_content);

        // free file path
        free(file_path);

        // get next stage path
        stage_path = strtok(NULL, "\n");
    }

    // free stage content
    free(stage_content);

    // display file head
    file_display(file_head); 

    // empty stage 
    if (write_file(STAGE_FILE, EMPTY) == 0) {
        printf("vcs: error: could not empty stage\n");
        exit(EXIT_FAILURE);
    }

    // insert commit to commit tree
    commit_tree_insert(commit_tree, hash(), timestamp(), message, 0, 0, file_head, NULL);

    // parse commit to commits file
    
}

void vcs_log(void) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
}
void vcs_log_content(void) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
}
void vcs_checkout(const char *hash) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
}
void vcs_checkout_current(void) {
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


