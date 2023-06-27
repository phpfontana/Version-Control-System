/**
 * @file actions.c
 * @brief VCS (Version Control System) main functions.
 *
 * This file contains the main functions of the VCS system,
 * including initialization, adding files to the stage, committing changes,
 * displaying commit logs, checking out to a specific commit, and showing
 * commit details.
 */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "data_structures.h"
# include "validations.h"
# include "files.h"
# include "actions.h"
# include "parser.h"

/**
 * @brief Initializes a new VCS repository.
 *
 * This function creates the necessary directory and files for a new VCS repository.
 * It checks if the ".vcs" directory already exists, and if not, creates it along
 * with the required files: "commits.txt", "stage.txt", "contents.txt", and "metadata.txt".
 *
 * @note This function terminates the program if the directory or files cannot be created.
 */
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

/**
 * @brief Adds a file to the VCS stage.
 *
 * This function adds a file to the VCS stage by appending its path to the "stage.txt" file
 * in the ".vcs" directory. It performs various validations such as checking if the ".vcs"
 * directory exists, if the provided path exists, and if the file has already been added to the stage.
 *
 * @param path The path of the file to be added to the stage.
 *
 * @note This function terminates the program if any validation fails or if the file cannot be appended to the stage.
 */
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

/**
 * @brief Commits the changes in the VCS stage.
 *
 * This function commits the changes in the VCS stage by creating a new commit with the provided message.
 * It validates the ".vcs" directory, checks if a message is provided, and verifies if the stage is not empty.
 * The function then parses the stage file, creates a new commit, updates the commit and file data structures,
 * and writes the changes to the necessary files.
 *
 * @param message The commit message.
 *
 * @note This function terminates the program if any validation fails or if the commit cannot be written successfully.
 */
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

/**
 * @brief Displays the commit log.
 *
 * This function displays the commit log by parsing the "commits.txt" file and printing the commits
 * in reverse order (from the most recent to the oldest). It validates the ".vcs" directory and
 * creates the commit data structure to hold the parsed commits.
 *
 * @note This function terminates the program if the ".vcs" directory is not valid or if the commits cannot be parsed.
 */
void vcs_log(void) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }

    // initialize commit head and files head
    CommitHead *commit_head = commit_create();

    // parse commits file to data structure
    parse_commits(commit_head);

    // display commits
    display_from_last(commit_head);

    // free memory
    commit_destroy(commit_head);

    // print success message
    printf("vcs: log successful\n");
}

/**
 * @brief Displays the commit log with contents.
 *
 * This function displays the commit log with contents by parsing the "commits.txt" file and printing
 * the commits in reverse order (from the most recent to the oldest) along with the contents of each commit.
 * It validates the ".vcs" directory and creates the commit data structure to hold the parsed commits.
 *
 * @note This function terminates the program if the ".vcs" directory is not valid or if the commits cannot be parsed.
 */
void vcs_log_content(void) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }

    // initialize commit head and files head
    CommitHead *commit_head = commit_create();

    // parse commits file to data structure
    parse_commits(commit_head);

    // display commits with contents
    display_from_last_with_contents(commit_head);

    // free memory
    commit_destroy(commit_head);

    // print success message
    printf("vcs: log successful\n");

}

/**
 * @brief Checks out to a specific commit.
 *
 * This function checks out to a specific commit by writing the commit and contents associated with
 * the provided commit hash. It validates the ".vcs" directory, parses the commits file, and verifies
 * if a hash is provided. If successful, it writes the commit and contents files to the current working directory.
 *
 * @param hash The hash of the commit to check out to.
 *
 * @note This function terminates the program if the ".vcs" directory is not valid, if no hash is provided,
 * or if the commit and contents cannot be written.
 */
void vcs_checkout(const char *hash) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
    // initialize commit head and files head
    CommitHead *commit_head = commit_create();

    // parse commits file to data structure
    parse_commits(commit_head);

    // verify if hash is NULL
    if (hash == NULL) {
        printf("vcs: error: no hash provided\n");
        exit(EXIT_FAILURE);
    }

    // checkout to hash
    write_commit_and_contents(commit_head, hash);

    // free memory
    commit_destroy(commit_head);
}

/**
 * @brief Checks out to the current commit.
 *
 * This function checks out to the current commit by writing the commit and contents associated with
 * the most recent commit. It validates the ".vcs" directory, parses the commits file, and checks out
 * to the most recent commit by writing the commit and contents files to the current working directory.
 * 
 * @note This function terminates the program if the ".vcs" directory is not valid or if the commit and contents cannot be written.
 */
void vcs_checkout_current(void) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }
    // initialize commit head and files head
    CommitHead *commit_head = commit_create();

    // parse commits file to data structure
    parse_commits(commit_head);

    // checkout to hash
    write_commit_and_contents(commit_head, commit_head->last->hash);

    // free memory
    commit_destroy(commit_head);
}

/**
 * @brief Displays the details of a specific commit.
 *
 * This function displays the details of a specific commit by parsing the "commits.txt" file and
 * finding the commit with the provided hash. It validates the ".vcs" directory, parses the commits,
 * and prints the commit information along with its contents.
 *
 * @param hash The hash of the commit to display.
 *
 * @note This function terminates the program if the ".vcs" directory is not valid, if no hash is provided,
 * or if the commit with the provided hash cannot be found.
 */
void vcs_show(const char *hash) {
    // validate .vcs directory
    if (validate_directory(VCS_DIRECTORY) == 0) {
        printf("vcs: error: .vcs was not initialized\n");
        exit(EXIT_FAILURE);
    }

    // initialize commit head and files head
    CommitHead *commit_head = commit_create();

    // parse commits file to data structure
    parse_commits(commit_head);

    // verify if hash is NULL
    if (hash == NULL) {
        printf("vcs: error: no hash provided\n");
        exit(EXIT_FAILURE);
    }

    // display commit and contents
    display_commit_and_contents(commit_head, hash);

    // free memory
    commit_destroy(commit_head);

}
