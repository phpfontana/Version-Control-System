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
# include "log.h"


int main() {
    
    vcs_log();
    return 0;
}

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