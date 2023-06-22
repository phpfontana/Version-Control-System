# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>
# include <sys/stat.h>
# include <time.h>
# include "add.h"
# include "vcs.h"
# include "init.h"
# include "commit.h"

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

int main(int argc, char const *argv[])
{
    // test
    extractContentFromFile(COMMITS_FILE, 155, 312, "test.txt");

    vcs_commit("testing commit");
    return 0;
}

// verifies if directory exists
int directory_exists(char *path) {
    int status = access(path, 0);
    if (status == 0)
        return 1;  // directory exists
    else
        return 0;  // directory does not exist
}

// verify if file exists
int file_exists(char *path) {
    int status = access(path, 0);
    if (status == 0)
        return 1;  // file exists
    else
        return 0;  // file does not exist
}

// validates directory
int validate_dir(void) {
    // verifies if .vcs folder exists
    if (directory_exists(VCS_FOLDER) == 0) {
        printf("vcs: error: .vcs folder does not exist\n");
        return 0;
    }

    // verifies if database files exist
    if (directory_exists(CONTENTS_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", CONTENTS_FILE);
        return 0;
    }

    if (directory_exists(COMMITS_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", COMMITS_FILE);
        return 0;
    }

    if (directory_exists(METADATA_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", METADATA_FILE);
        return 0;
    }

    if (directory_exists(STAGE_FILE) == 0) {
        printf("vcs: error: %s does not exist\n", STAGE_FILE);
        return 0;
    }

    return 1;
}

// creates file
void create_file(char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create file %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// verifies if file is empty
int is_empty(char *path) {
    FILE *file = fopen(path, "r");  // open file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open file\n");
        exit(EXIT_FAILURE);
    }

    // get file size
    fseek(file, 0, SEEK_END);  // set pointer to end of file
    int size = ftell(file);  // get file size
    
    // close file
    fclose(file);

    // return 1 if file is empty, 0 otherwise 
    if (size == 0)
        return 1;  
    else
        return 0;
}

// generates a random hash
char *get_hash() {
    char *hash = malloc(41 * sizeof(char));
    if (hash == NULL) {
        printf("Error allocating memory for hash\n");
        exit(EXIT_FAILURE);
    }

    const char *hexChars = "0123456789ABCDEF";
    srand(time(NULL));

    for (int i = 0; i < 40; i++) {
        hash[i] = hexChars[rand() % 16];
    }
    hash[40] = '\0';

    return hash;
}

// generate timestamp
char *get_timestamp() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}

/// Initialize empty commit
Commit *init_commit(char* message) {
    // Initialize Commit struct
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory for commit\n");
        exit(EXIT_FAILURE);
    }
    commit->hash = get_hash();
    commit->message = message;
    commit->date = get_timestamp();
    commit->byte_start = 0;
    commit->byte_end = 0;
    commit->files = NULL;
    commit->next = NULL;
    commit->prev = NULL;

    return commit;
}

// Insert new file into commit
void insert_file(Commit *commit, char *file_path) {
    // Initialize Files struct
    Files *new_file = (Files *) malloc(sizeof(Files));
    if (new_file == NULL) {
        printf("vcs: error: could not allocate memory for file\n");
        exit(EXIT_FAILURE);
    }
    new_file->file_path = strdup(file_path);
    new_file->byte_start = -1;
    new_file->byte_end = -1;
    new_file->next = NULL;
    new_file->prev = NULL;

    // Insert file into commit
    Files *current = commit->files;
    if (current == NULL) {  // If commit is empty
        commit->files = new_file;  // Insert file into commit
    } else {  // If commit is not empty
        while (current->next != NULL) {  // Go to last file
            current = current->next;
        }  
        current->next = new_file;  // Insert file into commit
        new_file->prev = current;  // Update file's previous pointer
    }
}

// Reads stage file and inserts files into commit
void read_stage(Commit *commit) {
    FILE *file = fopen(STAGE_FILE, "r");  // Open file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open file\n");
        exit(EXIT_FAILURE);
    }

    char buffer[256];  // Buffer to store file paths
    while (fgets(buffer, sizeof(buffer), file)) {  // Read file line by line
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character
        insert_file(commit, buffer);  // Insert file into commit
    }

    fclose(file);
}

// prints files in commit
void printFiles(struct files *head) {
    struct files *current = head;
    while (current != NULL) {
        printf("File Path: %s\n", current->file_path);
        printf("Byte Start: %d\n", current->byte_start);
        printf("Byte End: %d\n", current->byte_end);
        printf("---------------------\n");
        current = current->next;
    }
}

// appends Files contents to contents.txt
void append_contents(Files *files) {
    FILE *file = fopen(CONTENTS_FILE, "a");  // Open file in append mode
    if (file == NULL) {
        printf("vcs: error: could not open file\n");
        exit(EXIT_FAILURE);
    }

    Files *current = files;
    while (current != NULL) 
    {
        FILE *file_to_copy = fopen(current->file_path, "r");  // Open file in read mode
        if (file_to_copy == NULL) {
            printf("vcs: error: could not open file\n");
            exit(EXIT_FAILURE);
        }

        // store byte start
        current->byte_start = ftell(file);
        // copy all file contents to contents.txt
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file_to_copy)) {
            fputs(buffer, file);
        }
        fputs("\n", file);
        fclose(file_to_copy);

        // store byte end
        current->byte_end = ftell(file) - 1;

        // go to next file
        current = current->next;

        // add newline character if there is a next file
        if (current != NULL) {  
            fputs("\n", file);
        }
    }
    fclose(file);
}

// Append commit to commits.txt
void append_commit(Commit *commit) {
    FILE *file = fopen(COMMITS_FILE, "a");  // Open file in append mode
    if (file == NULL) {
        printf("vcs: error: could not open file\n");
        exit(EXIT_FAILURE);
    }
    // set pointer to end of file
    fseek(file, 0, SEEK_END);

    // store byte start
    commit->byte_start = ftell(file);

    // Write commit to file
    fprintf(file, "ID: %s\n", commit->hash);
    fprintf(file, "Date: %s\n", commit->date);
    fprintf(file, "Files:\n");
    Files *current = commit->files;
    while (current != NULL) {
        fprintf(file, "%s %d %d\n", current->file_path, current->byte_start, current->byte_end);
        current = current->next;
    }
    fprintf(file, "Message: %s\n", commit->message);

    // Write commit separator
    fprintf(file, "---------------------\n");
    
    // store byte end
    commit->byte_end = ftell(file) - 1;

    fclose(file);
}

// append bytes to metadata.txt
void append_metadata(Commit *commit) {
    FILE *file = fopen(METADATA_FILE, "a");  // Open file in append mode
    if (file == NULL) {
        printf("vcs: error: could not open file\n");
        exit(EXIT_FAILURE);
    }
    // set pointer to end of file
    fseek(file, 0, SEEK_END);

    // Write commit to file
    fprintf(file, "%s %d %d\n", commit->hash, commit->byte_start, commit->byte_end);

    fclose(file);
}

void vcs_commit(char *message) 
{
    // validates directory
    if (validate_dir() == 0)
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
    create_file(STAGE_FILE);

    // free memory
    free(commit);
}

// TODO
// Modularize
// fix metadata.tx