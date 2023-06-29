# include <stdio.h> 
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <time.h>
# include <unistd.h>
# include <sys/stat.h>

# include "data_structures.h"

// Function prototypes for tree operations
CommitTree* create_commit_tree() {
    CommitTree* tree = (CommitTree*) malloc(sizeof(CommitTree));  // allocate memory for the tree
    if (tree == NULL) {  // if the tree is NULL
        printf("Error: Could not allocate memory for the tree.\n");  // print an error message
        exit(1);  // exit the program
    }
    tree->root = NULL;  // set the root to NULL
    return tree;     // return the tree
}
Commit* create_commit(char* hash, char* date, const char* message, int start_byte, int end_byte) {
    Commit* commit = (Commit*) malloc(sizeof(Commit));  // allocate memory for the commit
    if (commit == NULL) {  // if the commit is NULL
        printf("Error: Could not allocate memory for the commit.\n");  // print an error message
        exit(1);  // exit the program
    }
    commit->hash = strdup(hash);  // set the hash
    commit->date = strdup(date);  // set the date
    commit->message = strdup(message);  // set the message
    commit->start_byte = start_byte;  // set the start byte
    commit->end_byte = end_byte;  // set the end byte
    commit->file = NULL;  // set the file to NULL
    commit->parent = NULL;  // set the parent to NULL
    commit->child = NULL;  // set the child to NULL
    commit->sibling = NULL;  // set the siblings to NULL

    return commit;  // return the commit
  
}
void insert_commit(CommitTree* tree, Commit* parent, Commit* child) {
        if (tree == NULL || child == NULL)
        return;

    if (parent == NULL) {
        // Inserting root commit
        tree->root = child;
        tree->current = child;
    } else {
        // Inserting child commit under parent commit
        child->parent = parent;
        if (parent->child == NULL) {
            parent->child = child;
        } else {
            Commit* sibling = parent->child;
            while (sibling->sibling != NULL) {
                sibling = sibling->sibling;
            }
            sibling->sibling = child;
        }
    }

    tree->size++;
}

void destroy_files(File* file) {
    while (file != NULL) {
        File* next = file->next;
        free(file->path);
        free(file);
        file = next;
    }
}

void destroy_commit_tree_helper(Commit* commit) {
    if (commit != NULL) {
        destroy_commit_tree_helper(commit->child);
        destroy_commit_tree_helper(commit->sibling);
        destroy_files(commit->file);
        destroy_commit(commit);
    }
}

void destroy_commit_tree(CommitTree* tree) {
    if (tree != NULL) {
        destroy_commit_tree_helper(tree->root);
        free(tree);
    }
}


// Function prototypes for file operations
File* create_file(const char* path, int start_byte, int end_byte) {
    File* file = (File*) malloc(sizeof(File));  // allocate memory for the file
    if (file == NULL) {  // if the file is NULL
        printf("Error: Could not allocate memory for the file.\n");  // print an error message
        exit(1);  // exit the program
    }
    file->path = strdup(path);  // set the path
    file->start_byte = start_byte;  // set the start byte
    file->end_byte = end_byte;  // set the end byte
    file->next = NULL;  // set the next to NULL
    file->prev = NULL;  // set the prev to NULL

    return file;  // return the file
}
void insert_file(Commit* commit, File* file) {
    if (commit == NULL || file == NULL)
        return;

    if (commit->file == NULL) {
        commit->file = file;
    } else {
        File* current = commit->file;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = file;
        file->prev = current;
    }
}
