#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "utils.h"
#include "actions.h"

/// FILE FUNCTIONS
// Opens a file
FILE* open_file(const char* path, const char* mode) {
    FILE* file = fopen(path, mode);
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", path);
        exit(EXIT_FAILURE);
    }
    return file;
}

// Closes a file
void close_file(FILE* file) {
    fclose(file);
}

// Reads a file and returns its contents
char* read_file_contents(FILE* file, int byte_start, int byte_end) {
    fseek(file, byte_start, SEEK_SET);  // sets the file pointer to the beginning of the file
    int size = byte_end - byte_start;
    char* contents = malloc(size + 1);  // allocates memory for the contents of the file
    fread(contents, size, 1, file);     // reads the file and stores its contents in the contents variable
    contents[size] = '\0';              // adds a null terminator to the end of the contents variable
    return contents;
} 


/// VCS INIT
// Verifies if directory exists
int directory_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

// Creates a directory
int create_directory(const char *path) {
    int status = mkdir(path, 0777);
    return (status == 0);
}

// Creates a file
void write_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create file %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

/// VCS ADD
// verify if file exists
int file_exists(const char* path)
{
    int status = access(path, F_OK);
    return status == 0;
}

// Verify if path was already added to file
int path_already_added(const char* file_path, const char* output_file) {
    FILE* file = fopen(output_file, "r");  // Opens file in read mode
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", output_file);
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {  // Reads file line by line
        line[strcspn(line, "\n")] = '\0';  // Remove the trailing newline character

        if (strcmp(line, file_path) == 0) {  // Compares file path with line
            fclose(file);
            free(line);
            return 1;  // Path was already added
        }
    }

    fclose(file);
    free(line);
    return 0;  // Path was not added
}

// add paths to files
void add_path_to_file(const char* file_path, char* output_file) {
    FILE* file = fopen(output_file, "a");  // opens file in append mode
    if (file == NULL) {
        printf("vcs: error: could not open file %s\n", output_file);
        exit(EXIT_FAILURE);
    }

    char* file_path_copy = strdup(file_path);  // creates copy of file paths
    if (file_path_copy == NULL) {
        printf("vcs: error: could not allocate memory for file paths\n");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(file_path_copy, ", ");  // gets first file path
    while (token != NULL) {
        // verifies if file exists
        if (file_exists(token) == 0) {
            printf("vcs: error: file %s does not exist\n", token);
            exit(EXIT_FAILURE);
        }
        if (path_already_added(token, output_file) == 1) {  // verifies if path was already added
            printf("vcs: error: file %s was already added\n", token);
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s\n", token);  // writes file path to file
        token = strtok(NULL, ", ");  // gets next file path
    }
    fclose(file);
    free(file_path_copy);
}

/// VCS COMMIT
// validates directory
int validate_directory(void) {
    // verifies if .vcs folder exists
    if (directory_exists(VCS_DIRECTORY) == 0) {
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
Commit *init_commit(const char* message) {
    // Initialize Commit struct
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory for commit\n");
        exit(EXIT_FAILURE);
    }
    char* message_copy = strdup(message);
    commit->hash = get_hash();
    commit->message = message_copy;;
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

/// LOG
// Function to read commits.txt and populate the Commit struct
void read_commits(const char* commits_file, Commit** head) {
    FILE* file = fopen(commits_file, "r");
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

// Function to print commit information from last to first commit
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

// Function to free the memory allocated for commit information
void free_commits(Commit* head) {
    Commit* current = head;
    while (current != NULL) {
        Commit* next = current->next;
        free(current->hash);
        free(current->date);
        free(current->message);
        free(current);
        current = next;
    }
}

// VCS CHECKOUT
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


/// VCS SHOW
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


