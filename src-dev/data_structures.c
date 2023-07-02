/**
 * @file data_structures.c
 *
 * @brief Implementation of Data Structures
 *
 * This file provides the implementation of data structures used in the VCS application,
 * such as the FileHead and CommitTree structures, along with their associated functions.
 *
 * @note This file assumes that the necessary header file (`data_structures.h`) is present in the project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include "data_structures.h"

// File functions

/**
 * @brief Creates a new FileHead structure.
 *
 * This function allocates memory for a new FileHead structure, initializes its members,
 * and returns a pointer to the created structure.
 *
 * @return A pointer to the newly created FileHead structure.
 */
FileHead* file_create(void) {
    FileHead* head = (FileHead*) malloc(sizeof(FileHead));
    if (head == NULL)
        return head;
    head->first = NULL;
    head->last = NULL;
    return head;
}

/**
 * @brief Inserts a file path into the FileHead structure.
 *
 * This function creates a new File structure, allocates memory for it, copies the provided file path,
 * and inserts it into the FileHead structure. If the provided file path is already present in the structure,
 * no insertion is performed.
 *
 * @param head The FileHead structure to insert the file path into.
 * @param path The file path to be inserted.
 */
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

/**
 * @brief Displays the file paths stored in the FileHead structure.
 *
 * This function traverses the FileHead structure and prints each file path to the standard output.
 *
 * @param head The FileHead structure to display the file paths from.
 */
void file_display(FileHead* head) {
    File* file = head->first;
    while (file != NULL) {
        printf("%s\n", file->path);
        file = file->next;
    }
}

/**
 * @brief Destroys the FileHead structure and frees the associated memory.
 *
 * This function frees the memory allocated for the FileHead structure and its associated File structures,
 * including the file paths stored within them.
 *
 * @param head The FileHead structure to be destroyed.
 */
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

/**
 * @brief Creates a new CommitTree structure.
 *
 * This function allocates memory for a new CommitTree structure, initializes its members,
 * and returns a pointer to the created structure.
 *
 * @return A pointer to the newly created CommitTree structure.
 */
CommitTree* commit_tree_create(void) {
    CommitTree* tree = (CommitTree*) malloc(sizeof(CommitTree));
    if (tree == NULL)
        return tree;
    tree->root = NULL;
    return tree;
}

/**
 * @brief Inserts a commit into the CommitTree structure.
 *
 * This function creates a new Commit structure, allocates memory for it, copies the provided commit information,
 * and inserts it into the CommitTree structure. The commit is added as a child of the last commit in the tree,
 * or as the root if the tree is empty.
 *
 * @param tree The CommitTree structure to insert the commit into.
 * @param hash The commit hash.
 * @param date The commit date.
 * @param message The commit message.
 * @param start_byte The start byte.
 * @param end_byte The end byte.
 * @param file The FileHead structure associated with the commit.
 * @param parent The parent Commit structure.
 */
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

/**
 * @brief Displays the commits stored in the CommitTree structure.
 *
 * This function traverses the CommitTree structure and prints the details of each commit,
 * including the commit hash, date, message, start and end bytes, parent commit hash, and associated files.
 *
 * @param tree The CommitTree structure to display the commits from.
 */
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

/**
 * @brief Destroys the CommitTree structure and frees the associated memory.
 *
 * This function frees the memory allocated for the CommitTree structure and its associated Commit structures,
 * including the commit information and the associated FileHead structures.
 *
 * @param tree The CommitTree structure to be destroyed.
 */
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

// ...

void printInfo(const char* message) {
    printf("[INFO] %s\n", message);
}

/**
 * @brief Prints an alert message.
 *
 * @param message The message to be printed.
 */
void printAlert(const char* message) {
    printf("[ALERT] %s\n", message);
}

/**
 * @brief Prints a warning message.
 *
 * @param message The message to be printed.
 */
void printWarning(const char* message) {
    printf("[WARNING] %s\n", message);
}

