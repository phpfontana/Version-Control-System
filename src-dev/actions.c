#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "utils.h"
#include "actions.h"

/// VCS INIT
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

/// VCS ADD
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

    // add file or files to staging area
    add_path_to_file(file_path, STAGE_FILE);

    // success message
    printf("vcs: file paths added to staging area\n");
}


/// VCS COMMIT
void vcs_commit(const char *message) 
{
    // validates directory
    if (validate_directory() == 0)
    {
        printf("vcs: error: .vcs was not initialized properly\n");
        exit(EXIT_FAILURE);
    }

    // verifies if staging area is empty
    if (is_empty(STAGE_FILE) == 1) {
        printf("vcs: error: staging area is empty\n");
        exit(EXIT_FAILURE);
    }

    // verifies if commit message is empty
    if (message == NULL) {
        printf("vcs: error: commit message is empty\n");
        exit(EXIT_FAILURE);
    }

    // initializes commit
    Commit *commit = init_commit(message);

    // read stage file and insert files into commit
    read_stage(commit);

    // write Files contents to contents.txt
    append_contents(commit->files);

    // append commit to commits.txt
    append_commit(commit);

    // append commit to metadata.txt
    append_metadata(commit);

    // rewrite stage file
    write_file(STAGE_FILE);

    // free memory
    free(commit);

    // success message
    printf("vcs: commit successful\n");
}

/// VCS LOG
void vcs_log()
{
    // Initialize the head of the linked list
    Commit* head = NULL;  

    // Read the commits.txt file and populate the linked list
    read_commits(COMMITS_FILE, &head);

    // Print the commits from last to first
    print_commits(head);

    // Free the memory allocated for the linked list
    free_commits(head);
}

/// VCS CHECKOUT
void vcs_checkout(const char *hash) {
    // Open metadata.txt file
    FILE *metadataFile = fopen(METADATA_FILE, "r");
    if (metadataFile == NULL) {
        printf("Error opening metadata.txt file\n");
        return;
    }

    // Search for the given hash in metadata.txt
    char line[100];
    char hashFromFile[41];
    int byteStart, byteEnd;
    int hashFound = 0;
    while (fgets(line, sizeof(line), metadataFile)) {
        sscanf(line, "%s %d %d", hashFromFile, &byteStart, &byteEnd);
        if (strcmp(hashFromFile, hash) == 0) {
            hashFound = 1;
            break;  // Found the hash, stop searching
        }
    }
    fclose(metadataFile);

    if (!hashFound) {
        printf("Hash not found in metadata.txt\n");
        return;
    }

    // Open commits.txt file
    FILE *commitsFile = fopen(COMMITS_FILE, "r");
    if (commitsFile == NULL) {
        printf("Error opening commits.txt file\n");
        return;
    }

    // Move file pointer to the start byte of the commit
    fseek(commitsFile, byteStart, SEEK_SET);

    // Print the commit ID
    printf("Commit ID: %s\n", hash);

    // Parse the files section
    Files *headFile = parseFiles(commitsFile, byteEnd);

    // Print the parsed files
    Files *currentFile = headFile;
    while (currentFile != NULL) {
        printf("File Path: %s, Start: %d, End: %d\n", currentFile->file_path, currentFile->byte_start, currentFile->byte_end);
        extractContentFromFile(CONTENTS_FILE, currentFile->byte_start, currentFile->byte_end, currentFile->file_path);
        currentFile = currentFile->next;
    }

    // Clean up: free the allocated memory
    freeFiles(headFile);

    // Close the commits.txt file
    fclose(commitsFile);
}

/// VCS SHOW
void vcs_show(const char *id) {
    // open metadata.txt
    FILE* metadata = fopen(METADATA_FILE, "r");
    if (metadata == NULL) {
        printf("vcs: error: metadata file not found\n");
        return;
    }

    // search for the id in metadata.txt
    char line[256];
    char hash[41];
    int byte_start, byte_end;
    int hash_found = 0;
    while (fgets(line, sizeof(line), metadata)) {
        sscanf(line, "%s %d %d", hash, &byte_start, &byte_end);
        if (strcmp(hash, hash) == 0) {
            hash_found = 1;
            break;  // Found the hash, stop searching
        }
    }
    fclose(metadata);

    if (!hash_found) {
        printf("vcs: error: hash not found in metadata.txt\n");
        return;
    }

    // extract the content from the file
    FILE* commits = fopen(COMMITS_FILE, "r");
    if (commits == NULL) {
        printf("vcs: error: commits file not found\n");
        return;
    }

    // move the file pointer to the start byte position
    fseek(commits, byte_start, SEEK_SET);
    Commit* current = NULL;

    char buffer[256];
    while (fgets(line, sizeof(line), commits) != NULL) {
        if (strncmp(line, "ID: ", 4) == 0) {
            // Create a new commit
            Commit* commit = malloc(sizeof(Commit));
            commit->hash = strdup(line + 4); // Skip "ID: "
            commit->hash[strcspn(commit->hash, "\n")] = '\0'; // Remove newline character
            commit->date = NULL;
            commit->message = NULL;
            commit->files = NULL;
            commit->next = NULL;
            commit->prev = NULL;

            current = commit;
        } else if (strncmp(line, "Date: ", 6) == 0) {
            // Set the commit date
            current->date = strdup(line + 6); // Skip "Date: "
            current->date[strcspn(current->date, "\n")] = '\0'; // Remove newline character
        } else if (strncmp(line, "Message: ", 9) == 0) {
            // Set the commit message
            current->message = strdup(line + 9); // Skip "Message: "
            current->message[strcspn(current->message, "\n")] = '\0'; // Remove newline character
        }

        }
        fclose(commits);

    // print the commit
    print_commits(current);

    // Open commits.txt file
    FILE *commitsFile = fopen(COMMITS_FILE, "r");
    if (commitsFile == NULL) {
        printf("Error opening commits.txt file\n");
        return;
    }

    // Move file pointer to the start byte of the commit
    fseek(commitsFile, byte_start, SEEK_SET);

    // Parse the files section
    Files *headFile = parseFiles(commitsFile, byte_end);

    // Print the parsed files
    Files *currentFile = headFile;
    while (currentFile != NULL) {
        printf("File Path: %s, Start: %d, End: %d\n", currentFile->file_path, currentFile->byte_start, currentFile->byte_end);
        extractContentFromFile_print(CONTENTS_FILE, currentFile->byte_start, currentFile->byte_end);
        currentFile = currentFile->next;
    }
}

int main(int argc, char const *argv[])
{
    vcs_show("B210F27B50A16B53DEEB17CA74FDC431ADCFDBFD");
    return 0;
}