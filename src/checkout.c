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

void freeFiles(Files *headFile) {
    Files *currentFile = headFile;
    while (currentFile != NULL) {
        Files *temp = currentFile;
        currentFile = currentFile->next;
        free(temp->file_path);
        free(temp);
    }
}

void extractContentFromFile(const char* file_path, int start_byte, int end_byte, const char* output_file) {
    FILE* input_file = fopen(file_path, "r");
    if (input_file == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }

    FILE* output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error creating output file: %s\n", output_file);
        fclose(input_file);
        return;
    }

    fseek(input_file, start_byte, SEEK_SET);  // Move file pointer to the start byte position

    char buffer[256];
    int bytes_remaining = end_byte - start_byte + 1;
    int bytes_to_read = bytes_remaining < sizeof(buffer) ? bytes_remaining : sizeof(buffer);

    while (bytes_remaining > 0 && fread(buffer, 1, bytes_to_read, input_file) > 0) {
        fwrite(buffer, 1, bytes_to_read, output);
        bytes_remaining -= bytes_to_read;
        bytes_to_read = bytes_remaining < sizeof(buffer) ? bytes_remaining : sizeof(buffer);
    }

    fclose(input_file);
    fclose(output);
}

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

int main() {
    vcs_checkout("E2E992D5DCDA2E3E266C368E0B354101E27DFBB0");
    return 0;
}