#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include "init.h"
# include "vcs.h"

Files *parseFiles(FILE *commitsFile, int endByte) {
    char line[1000];
    Files *headFile = NULL;
    Files *prevFile = NULL;
    int bytePosition = ftell(commitsFile);

    while (fgets(line, sizeof(line), commitsFile)) {
        if (strcmp(line, "Files:\n") == 0) {
            break;
        }
    }

    while (fgets(line, sizeof(line), commitsFile)) {
        if (strstr(line, "Message:") != NULL || bytePosition >= endByte) {
            break;
        }

        char *token = strtok(line, " ");
        if (token == NULL) {
            continue;
        }

        Files *file = (Files *)malloc(sizeof(Files));
        file->file_path = strdup(token);

        token = strtok(NULL, " ");
        if (token == NULL) {
            continue;
        }
        file->byte_start = atoi(token);

        token = strtok(NULL, " ");
        if (token == NULL) {
            continue;
        }
        file->byte_end = atoi(token);

        file->next = NULL;

        if (prevFile != NULL) {
            prevFile->next = file;
        } else {
            headFile = file;
        }

        prevFile = file;

        bytePosition = ftell(commitsFile);
    }

    return headFile;
}

void extractContentFromFile(const char* file_path, int start_byte, int end_byte) {
    FILE* input_file = fopen(file_path, "r");
    if (input_file == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }

    fseek(input_file, start_byte, SEEK_SET);  // Move file pointer to the start byte position

    char buffer[256];  // Buffer to read the file in chunks
    int bytes_remaining = end_byte - start_byte + 1;  // Number of bytes to read
    int bytes_to_read = bytes_remaining < sizeof(buffer) ? bytes_remaining : sizeof(buffer);  // Number of bytes to read in each chunk

    while (bytes_remaining > 0 && fread(buffer, 1, bytes_to_read, input_file) > 0) {  
        bytes_remaining -= bytes_to_read;
        bytes_to_read = bytes_remaining < sizeof(buffer) ? bytes_remaining : sizeof(buffer);
    }
    // print the buffer to terminal
    printf("%s", buffer);

    // Add a newline at the end of the file
    buffer[bytes_to_read] = '\0';  // Null-terminate the buffer

    fclose(input_file);
}

void print_commits(Commit* head) {
    Commit* current = head;
    while (current != NULL) {
        printf("Commit: %s\n", current->hash);
        printf("Date: %s\n", current->date);
        printf("Message: %s\n", current->message);
        printf("\n");

        current = current->next;
    }
}


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
        extractContentFromFile(CONTENTS_FILE, currentFile->byte_start, currentFile->byte_end);
        currentFile = currentFile->next;
    }


}

int main(int argc, char const *argv[])
{
    vcs_show("A491AB363FC42BCDD22CC4CCA784837E336EE627");
    return 0;
}
