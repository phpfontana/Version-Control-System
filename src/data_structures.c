# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"
# include "parser.h"

// function prototypes
CommitHead* commit_create() {
    CommitHead* head = (CommitHead*)malloc(sizeof(CommitHead));
    if (head == NULL)
        return head;
    head->first = NULL;
    head->last = NULL;
    return head;
}
void commit_insert(CommitHead* head, char* hash, char* date, const char* message, int start_byte, int end_byte) {
    Commit* commit = (Commit*)malloc(sizeof(Commit));
    if (commit == NULL)
        return;
    
    commit->hash = (char*)malloc(sizeof(char) * (strlen(hash) + 1));
    strcpy(commit->hash, hash);
    commit->date = (char*)malloc(sizeof(char) * (strlen(date) + 1));
    strcpy(commit->date, date);
    commit->message = (char*)malloc(sizeof(char) * (strlen(message) + 1));
    strcpy(commit->message, message);
    commit->start_byte = start_byte;
    commit->end_byte = end_byte;

    commit->next = NULL;
    commit->prev = NULL;
    if (head->first == NULL) {
        head->first = commit;
        head->last = commit;
    } else {
        head->last->next = commit;
        commit->prev = head->last;
        head->last = commit;
    }
}

void commit_display(CommitHead* head) {
    Commit* aux = head->first;
    while (aux != NULL) {
        printf("Hash: %s\n", aux->hash);
        printf("Date: %s\n", aux->date);
        printf("Message: %s\n", aux->message);
        printf("Start byte: %d\n", aux->start_byte);
        printf("End byte: %d\n", aux->end_byte);
        printf("\n");
        aux = aux->next;
    }
}

void commit_destroy(CommitHead* head) {
    Commit* aux = head->first;
    while (aux != NULL) {
        Commit* t = aux->next;
        free(aux->hash);
        free(aux->date);
        free(aux->message);
        free(aux);
        aux = t;
    }
    free(head);
}

FileHead* file_create() {
    FileHead* head = (FileHead*)malloc(sizeof(FileHead));
    if (head == NULL)
        return head;
    head->first = NULL;
    head->last = NULL;
    return head;
}
void file_insert(FileHead* head, const char* path, int start_byte, int end_byte) {
    File* file = (File*)malloc(sizeof(File));
    if (file == NULL)
        return;
    
    file->path = (char*)malloc(sizeof(char) * (strlen(path) + 1));
    strcpy(file->path, path);
    file->start_byte = start_byte;
    file->end_byte = end_byte;

    file->next = NULL;
    file->prev = NULL;
    if (head->first == NULL) {
        head->first = file;
        head->last = file;
    } else {
        head->last->next = file;
        file->prev = head->last;
        head->last = file;
    }
}
void file_display(FileHead* head) {
    File* aux = head->first;
    while (aux != NULL) {
        printf("Path: %s\n", aux->path);
        printf("Start byte: %d\n", aux->start_byte);
        printf("End byte: %d\n", aux->end_byte);
        printf("\n");
        aux = aux->next;
    }
}

void file_destroy(FileHead* head) {
    File* aux = head->first;
    while (aux != NULL) {
        File* t = aux->next;
        free(aux->path);
        free(aux);
        aux = t;
    }
    free(head);
}

void display_commits(CommitHead* commit_head) {
    Commit* aux = commit_head->first;
    while (aux != NULL) {
        printf("Hash: %s\n", aux->hash);
        printf("Date: %s\n", aux->date);
        printf("Message: %s\n", aux->message);
        printf("Start byte: %d\n", aux->start_byte);
        printf("End byte: %d\n", aux->end_byte);
        printf("\n");

        // display files
        File* file_aux = aux->file_head->first;
        while (file_aux != NULL) {
            printf("Path: %s\n", file_aux->path);
            printf("Start byte: %d\n", file_aux->start_byte);
            printf("End byte: %d\n", file_aux->end_byte);
            printf("\n");
            file_aux = file_aux->next;
        }
        aux = aux->next;
    }
}

void display_from_last(CommitHead* commit_head){
    Commit* aux = commit_head->last;
    while (aux != NULL) {
        printf("commit %s\n", aux->hash);
        printf("Date:   %s\n", aux->date);
        printf("\n");
        printf("\t%s\n", aux->message);
        printf("\n");
        aux = aux->prev;
    }
}

void display_from_last_with_contents(CommitHead* commit_head) {
    Commit* aux = commit_head->last;
    while (aux != NULL) {
        printf("commit %s\n", aux->hash);
        printf("Date:   %s\n", aux->date);
        printf("\n");
        printf("\t%s\n", aux->message);
        printf("\n");

        // display files
        File* file_aux = aux->file_head->first;
        // iterate through files
        while (file_aux != NULL) {
            // display file contents
            printf("path %s\n", file_aux->path);
            printf("contents: \n");
            print_text_between_bytes(file_aux->start_byte, file_aux->end_byte);
            printf("\n");

            // next file
            file_aux = file_aux->next;
        }
        aux = aux->prev;
    }
}

void display_commit_and_contents(CommitHead* commit_head, const char* hash) {
    Commit* aux = commit_head->first;
    while (aux != NULL) {
        if (strcmp(aux->hash, hash) == 0) {
            printf("commit %s\n", aux->hash);
            printf("Date:   %s\n", aux->date);
            printf("\n");
            printf("\t%s\n", aux->message);
            printf("\n");

            // display files
            File* file_aux = aux->file_head->first;
            while (file_aux != NULL) {
                printf("path %s\n", file_aux->path);
                // display file contents
                printf("contents: \n");
                print_text_between_bytes(file_aux->start_byte, file_aux->end_byte);

                printf("\n");

                file_aux = file_aux->next;
            }
            return;
        }
        aux = aux->next;
    }
    printf("Commit not found!\n");
}

void write_commit_and_contents(CommitHead* commit_head, const char* hash) {
    Commit* aux = commit_head->first;
    while (aux != NULL) {
        if (strcmp(aux->hash, hash) == 0) {
            FILE* file = fopen("commit.txt", "w");
            if (file == NULL) {
                printf("Error opening file!\n");
                return;
            }
            fprintf(file, "Hash: %s\n", aux->hash);
            fprintf(file, "Date: %s\n", aux->date);
            fprintf(file, "Message: %s\n", aux->message);
            fprintf(file, "Start byte: %d\n", aux->start_byte);
            fprintf(file, "End byte: %d\n", aux->end_byte);
            fprintf(file, "\n");

            // display files
            File* file_aux = aux->file_head->first;
            while (file_aux != NULL) {
                fprintf(file, "Path: %s\n", file_aux->path);
                // display file contents
                write_text_between_bytes(file_aux->start_byte, file_aux->end_byte, file_aux->path);

                fprintf(file, "\n");

                file_aux = file_aux->next;
            }
            fclose(file);
            return;
        }
        aux = aux->next;
    }
    printf("Commit not found!\n");
}
