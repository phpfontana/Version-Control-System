#ifndef __DATA_STRUCTURES_H__
#define __DATA_STRUCTURES_H__

typedef struct commit_tree CommitTree;
typedef struct commit Commit;
typedef struct file File;

struct commit {
    char *hash;
    char* date;
    char* message;
    int start_byte;
    int end_byte;
    File* file;
    Commit* parent;
    Commit* child;
    Commit* sibling;
};

struct file {
    char* path;
    int start_byte;
    int end_byte;
    File* next;
    File* prev;
};

struct commit_tree {
    Commit* root;
    Commit* current;
    int size;
};

// Function prototypes for tree operations
CommitTree* create_commit_tree();
Commit* create_commit(char* hash, char* date, const char* message, int start_byte, int end_byte);
void insert_commit(CommitTree* tree, Commit* parent, Commit* child);
void destroy_commit_tree(CommitTree* tree);

// Function prototypes for file operations
File* create_file(const char* path, int start_byte, int end_byte);
void insert_file(Commit* commit, File* file);
void destroy_files(File* file);

#endif
