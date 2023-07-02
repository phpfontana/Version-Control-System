# include <stdio.h> 
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <time.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"

// File functions
FileHead* file_create(void) {
    FileHead* head = (FileHead*) malloc(sizeof(FileHead));
    if (head == NULL)
        return head;
    head->first = NULL;
    head->last = NULL;
    return head;
}

void file_insert(FileHead* head, const char* path) {
    File* file = (File*) malloc(sizeof(File));
    if (file == NULL)
        return;
    file->path = strdup(path);
    file->next = NULL;
    if (head->first == NULL) {
        head->first = file;
        head->last = file;
    } else {
        head->last->next = file;
        head->last = file;
    }
}

void file_display(FileHead* head) {
    File* file = head->first;
    while (file != NULL) {
        printf("%s\n", file->path);
        file = file->next;
    }
}

void file_destroy(FileHead* head) {
    File* file = head->first;
    while (file != NULL) {
        File* temp = file;
        file = file->next;
        free(temp->path);
        free(temp);
    }
    free(head);
}

// Commit functions
CommitTree* commit_tree_create(void) {
    CommitTree* tree = (CommitTree*) malloc(sizeof(CommitTree));
    if (tree == NULL)
        return tree;
    tree->root = NULL;
    return tree;
}
void commit_tree_insert(CommitTree *tree, const char* hash, const char* date, const char* message, int start_byte, int end_byte, FileHead* file, Commit* parent) {
    Commit* commit = (Commit*) malloc(sizeof(Commit));
    if (commit == NULL)
        return;
    commit->hash = strdup(hash);
    commit->date = strdup(date);
    commit->message = strdup(message);
    commit->start_byte = start_byte;
    commit->end_byte = end_byte;
    commit->file = file;
    commit->parent = parent;
    commit->child = NULL;
    if (tree->root == NULL) {
        tree->root = commit;
    } else {
        Commit* temp = tree->root;
        while (temp->child != NULL)
            temp = temp->child;
        temp->child = commit;
    }
}
void commit_tree_display(CommitTree* tree) {
    Commit* commit = tree->root;
    while (commit != NULL) {
        printf("hash: %s\n", commit->hash);
        printf("date: %s\n", commit->date);
        printf("message: %s\n", commit->message);
        printf("start_byte: %d\n", commit->start_byte);
        printf("end_byte: %d\n", commit->end_byte);
        printf("parent: %s\n", commit->parent == NULL ? "NULL" : commit->parent->hash);
        file_display(commit->file);
        commit = commit->child;
    }
}
void commit_tree_destroy(CommitTree* tree) {
    Commit* commit = tree->root;
    while (commit != NULL) {
        Commit* temp = commit;
        commit = commit->child;
        free(temp->hash);
        free(temp->date);
        free(temp->message);
        file_destroy(temp->file);
        free(temp);
    }
    free(tree);
}


