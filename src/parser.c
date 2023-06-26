# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# include "validations.h"
# include "files.h"
# include "parser.h"
# include "utils.h"
# include "data_structures.h"

void parse_stage(FileHead *head) {
    FILE* stage_file = open_file(STAGE_FILE, "r");

    // buffer for reading lines
    char buffer[256];

    // read lines
    while (fgets(buffer, sizeof(buffer), stage_file)) {
        // remove newline character
        buffer[strcspn(buffer, "\n")] = 0; 
        
        // read file path contents
        char *content = read_file(buffer);

        // append to contents file
        FILE* contents_file = open_file(CONTENTS_FILE, "a");
        int start_byte = ftell(contents_file);
        fprintf(contents_file, "%s\n", content);
        int end_byte = ftell(contents_file) - 1;
        fclose(contents_file);

        // store file path, byte start and end in Files linked list
        file_insert(head, buffer, start_byte, end_byte);
    }

    fclose(stage_file);
}

void parse_commit_file(CommitHead *commit_head, FileHead *file_head, const char *message) {
    // open commit file for appending
    FILE* commit_file = open_file(COMMITS_FILE, "a");

    // get start byte
    int start_byte = ftell(commit_file);
    // append commit to file
    fprintf(commit_file, "%s\n", hash());
    fprintf(commit_file, "%s\n", timestamp());
    File* current = file_head->first;
    while (current != NULL) {
        fprintf(commit_file, "%s %d %d", current->path, current->start_byte, current->end_byte);
        if (current->next != NULL) {
            fprintf(commit_file, ", ");
        }
        current = current->next;
    }
    // remove last comma
    
    fprintf(commit_file, "\n");
    fprintf(commit_file, "%s\n", message);
    
    // get end byte
    int end_byte = ftell(commit_file) - 1;

    // close commit file
    fclose(commit_file);

    // insert commit to commit linked list
    commit_insert(commit_head, hash(), timestamp(), message, start_byte, end_byte);
    
    // append to metadata file
    FILE* metadata_file = open_file(METADATA_FILE, "a");
    fprintf(metadata_file, "%s %d %d\n", commit_head->first->hash, start_byte, end_byte);
    fclose(metadata_file);
}

void parse_commits(CommitHead *commit_head) {
    // read metadata file
    FILE* metadata_file = open_file(METADATA_FILE, "r");

    // buffer for reading lines
    char buffer[256];

    // read lines
    while (fgets(buffer, sizeof(buffer), metadata_file)) {
        // remove newline character
        buffer[strcspn(buffer, "\n")] = 0; 
        
        // split line by space
        char *hash = strtok(buffer, " ");
        char *start_byte_str = strtok(NULL, " ");
        char *end_byte_str = strtok(NULL, " ");

        // convert start and end byte strings to integers
        int start_byte = atoi(start_byte_str);
        int end_byte = atoi(end_byte_str);

        // open commits file
        FILE* commits_file = open_file(COMMITS_FILE, "r");

        // move file pointer to start byte
        fseek(commits_file, start_byte, SEEK_SET);

        // read hash
        char hash_buffer[256];  // buffer for reading hash
        fgets(hash_buffer, sizeof(hash_buffer), commits_file);  // read hash
        hash_buffer[strcspn(hash_buffer, "\n")] = 0;  // remove newline character

        // read date
        char date_buffer[256];
        fgets(date_buffer, sizeof(date_buffer), commits_file);
        date_buffer[strcspn(date_buffer, "\n")] = 0;

        // read files path, start byte, and end byte
        char files_buffer[256];
        fgets(files_buffer, sizeof(files_buffer), commits_file);
        files_buffer[strcspn(files_buffer, "\n")] = 0;

        // read message
        char message_buffer[256];
        fgets(message_buffer, sizeof(message_buffer), commits_file);
        message_buffer[strcspn(message_buffer, "\n")] = 0;

        // close commits file
        fclose(commits_file);

        // create file head
        FileHead *file_head = file_create();

        // process files_buffer
        char *file_start = files_buffer;
        char *file_end = strchr(files_buffer, ',');
        while (file_end != NULL) {
            // calculate the length of the file path
            size_t path_length = file_end - file_start;
            // allocate memory for the file path
            char *file_path = malloc(path_length + 1);
            // copy the file path
            strncpy(file_path, file_start, path_length);
            file_path[path_length] = '\0';

            // move the file_start pointer to the next file
            file_start = file_end + 1;
            // find the next file_end pointer
            file_end = strchr(file_start, ',');

            // split the file information by space
            char *file_info = strtok(file_path, " ");
            char *start_byte_token = strtok(NULL, " ");
            char *end_byte_token = strtok(NULL, " ");

            // convert start and end byte strings to integers
            int file_start_byte = atoi(start_byte_token);
            int file_end_byte = atoi(end_byte_token);

            // insert file to file linked list
            file_insert(file_head, file_info, file_start_byte, file_end_byte);

            // free dynamically allocated memory
            free(file_path);
        }

        // process the last file in files_buffer
        size_t path_length = strlen(file_start);
        char *file_path = malloc(path_length + 1);
        strcpy(file_path, file_start);

        // split the file information by space
        char *file_info = strtok(file_path, " ");
        char *start_byte_token = strtok(NULL, " ");
        char *end_byte_token = strtok(NULL, " ");

        // convert start and end byte strings to integers
        int file_start_byte = atoi(start_byte_token);
        int file_end_byte = atoi(end_byte_token);

        // insert file to file linked list
        file_insert(file_head, file_info, file_start_byte, file_end_byte);

        // insert commit to commit linked list
        commit_insert(commit_head, hash_buffer, date_buffer, message_buffer, start_byte, end_byte);
        // assign file head to latest commit
        commit_head->last->file_head = file_head;

        printf("--------\n");
    }
    fclose(metadata_file);
}

void print_text_between_bytes(int byte_start, int byte_end) {
    // Open constants.txt file
    FILE* file = fopen(CONTENTS_FILE, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Move file pointer to byte_start
    fseek(file, byte_start, SEEK_SET);

    // Calculate the size of the text to be read
    size_t text_size = byte_end - byte_start;

    // Allocate memory for the text buffer
    char* buffer = (char*)malloc(text_size);
    if (buffer == NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    // Read the text from the file
    size_t bytes_read = fread(buffer, sizeof(char), text_size, file);
    buffer[bytes_read] = '\0';  // Add null terminator to the end of the buffer

    // Print the text
    printf("%s\n",buffer);

    // Free memory and close the file
    free(buffer);
    fclose(file);
}

void write_text_between_bytes(int byte_start, int byte_end, const char *path) {
    // Open contents.txt file
    FILE* input_file = fopen(CONTENTS_FILE, "r");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return;
    }

    // Move file pointer to byte_start
    fseek(input_file, byte_start, SEEK_SET);

    // Calculate the size of the contents to be read
    size_t contents_size = byte_end - byte_start;

    // Allocate memory for the contents buffer
    char* buffer = (char*)malloc(contents_size);
    if (buffer == NULL) {
        printf("Memory allocation error.\n");
        fclose(input_file);
        return;
    }

    // Read the contents from the input file
    size_t bytes_read = fread(buffer, sizeof(char), contents_size, input_file);
    buffer[bytes_read] = '\0';  // Add null terminator to the end of the buffer

    // Close the input file
    fclose(input_file);

    // Open the output file for writing
    FILE* output_file = fopen(path, "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        free(buffer);
        return;
    }

    // Write the contents to the output file
    fputs(buffer, output_file);

    // Close the output file
    fclose(output_file);

    // Free memory
    free(buffer);

    printf("Contents copied from byte %d to byte %d to file: %s\n", byte_start, byte_end, path);
}
