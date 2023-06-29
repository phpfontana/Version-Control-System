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

    // Create .vcs/contents.txt file
    if (write_file(CONTENTS_FILE, EMPTY) == 0) {
        printf("vcs: error: could not create .vcs/contents.txt file\n");
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