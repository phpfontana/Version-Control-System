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

        printf("Hash: %s\n", hash);
        printf("Start Byte: %d\n", start_byte);
        printf("End Byte: %d\n", end_byte);

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

        printf("Parsed Hash: %s\n", hash_buffer);
        printf("Parsed Date: %s\n", date_buffer);
        printf("Parsed Files: %s\n", files_buffer);
        printf("Parsed Message: %s\n", message_buffer);

        // close commits file
        fclose(commits_file);

        // create file head
        FileHead *file_head = file_create();
        
        // split files_buffer by comma
        char *file = strtok(files_buffer, ",");  
        while (file != NULL) {
            
            printf("Files Buffer: %s\n", files_buffer);
            // split files_buffer by space
            char *path = strtok(files_buffer, " ");
            printf("Files Buffer: %s\n", files_buffer);
            char *start_byte_token = strtok(NULL, " ");
            printf("Files Buffer: %s\n", files_buffer);
            char *end_byte_token = strtok(NULL, " ");
            printf("Files Buffer: %s\n", files_buffer);

            // convert start and end byte strings to integers
            int file_start_byte = atoi(start_byte_token);
            int file_end_byte = atoi(end_byte_token);

            // create copies of the path string
            char *file_path = strdup(path);

            printf("Parsed File: %s\n", file_path);
            printf("Parsed File Start Byte: %d\n", file_start_byte);
            printf("Parsed File End Byte: %d\n", file_end_byte);

            // insert file to file linked list
            file_insert(file_head, file_path, file_start_byte, file_end_byte);

            // get next file
            file = strtok(NULL, ",");
            printf("Files Buffer: %s\n", files_buffer);

            // free dynamically allocated memory
            free(file_path);
        }

        // insert commit to commit linked list
        commit_insert(commit_head, hash_buffer, date_buffer, message_buffer, start_byte, end_byte);
        // assign file head to latest commit
        commit_head->last->file_head = file_head;

        printf("--------\n");
    }
    fclose(metadata_file);
}
