# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <time.h>

# include "utils.h"
# include "data_structures.h"
# include "validations.h"
# include "files.h"
# include "parse.h"

// Parse file paths to stage file
void parse_file_paths_to_stage(const char* file_path) {
    // Open stage file in append mode
    FILE *stage_file = open_file(STAGE_FILE, "a");

    // Copy file paths
    char *file_path_copy = strdup(file_path);
    if (file_path_copy == NULL) {
        printf("vcs: error: could not allocate memory for file path\n");
        exit(EXIT_FAILURE);
    }

    // Get first file path
    char *token = strtok(file_path_copy, ", ");
    if (token == NULL) {
        printf("vcs: error: no file paths provided\n");
        exit(EXIT_FAILURE);
    }

    // Loop through file paths
    while (token != NULL) {
        // verify if file exists
        if (!file_exists(token)) {
            printf("vcs: error: file '%s' does not exist\n", token);
            exit(EXIT_FAILURE);
        }

        // verify if file is already staged
        else if (path_is_staged(token)) {
            printf("vcs: error: file '%s' is already staged\n", token);
            exit(EXIT_FAILURE);
        }

        // print file path to stage file
        fputs(token, stage_file);

        // get next file path
        token = strtok(NULL, ", ");
    }

    // close stage file
    close_file(stage_file);

    // free file path copy
    free(file_path_copy);
}

// Parse Stage to Files
void parse_stage_to_files(Commit* commit) {
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
}

// Parse Files to Contents
void parse_files_to_contents (Files* file) {
    // Open contents file in append mode
    FILE *contents_file = open_file(CONTENTS_FILE, "a");

    // first file from commit
    Files* current = file;

    // start byte for file
    current->byte_start = ftell(contents_file);

    // loop through files
    while (current != NULL) {
        // open file in read mode
        FILE *file_to_read = open_file(current->file_path, "r");

        // buffer for reading file contents
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file_to_read)) {
            fputs(buffer, contents_file);
        }
        fputs("\n", contents_file);

        // close file
        close_file(file_to_read);

        // end byte for file
        current->byte_end = ftell(contents_file) - 1;

        // next file
        current = current->next;

        // add newline character if there is a next file
        if (current != NULL) {
            fputs("\n", contents_file);
        }
    }

}

// Parse Commit to commit file
void parse_commit_to_commit_file(Commit* commit) {
    // Open commit file in append mode
    FILE *commit_file = open_file(COMMITS_FILE, "a");

    // start byte for commit
    commit->byte_start = ftell(commit_file);

    // Write commit to file
    fprintf(commit_file, "ID: %s\n", commit->hash);
    fprintf(commit_file, "Date: %s\n", commit->date);
    fprintf(commit_file, "Files:\n");
    Files *current = commit->files;
    while (current != NULL) {
        fprintf(commit_file, "%s %d %d\n", current->file_path, current->byte_start, current->byte_end);
        current = current->next;
    }
    fprintf(commit_file, "Message: %s\n", commit->message);

    // Write commit separator
    fprintf(commit_file, "---------------------\n");
    
    // store byte end
    commit->byte_end = ftell(commit_file) - 1;

    fclose(commit_file);
}

// Parse Commit to metadata file
void parse_commit_to_metadata_file(Commit* commit) {
    // Open metadata file in append mode
    FILE *metadata_file = open_file(METADATA_FILE, "a");

    // Write commit to file
    fprintf(metadata_file, "%s %d %d\n", commit->hash, commit->byte_start, commit->byte_end);

    // close metadata file
    fclose(metadata_file);
}

// Parse commit file to commit
void parse_commit_file_to_commit(Commit** head) {
    FILE* file = fopen(COMMITS_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to open commits file\n");
        return;
    }

    char line[256];
    Commit* current = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "ID: ", 4) == 0) {
            // Create a new commit
            Commit* commit = malloc(sizeof(Commit));
            commit->hash = strdup(line + 4); // Skip "ID: "
            commit->hash[strcspn(commit->hash, "\n")] = '\0'; // Remove newline character
            commit->date = NULL;
            commit->message = NULL;
            commit->next = NULL;
            commit->prev = NULL;

            // Insert the commit at the beginning of the linked list
            if (*head == NULL) {
                *head = commit;
            } else {
                commit->next = *head;
                (*head)->prev = commit;
                *head = commit;
            }

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

    fclose(file);
}

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

Files *parseFiles_(FILE *commitsFile, int endByte) {
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

void extractContentFromFile_print(const char* file_path, int start_byte, int end_byte) {
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

void print_commits_(Commit* head) {
    Commit* current = head;
    while (current != NULL) {
        printf("Commit: %s\n", current->hash);
        printf("Date: %s\n", current->date);
        printf("Message: %s\n", current->message);
        printf("\n");

        current = current->next;
    }
}

