#ifndef __DATA_STRUCTURES_H__
#define __DATA_STRUCTURES_H__

typedef struct commit_tree CommitTree;
typedef struct commit Commit;
typedef struct file_head FileHead;
typedef struct file File;

struct commit {
    char* hash;             // commit hash
    char* date;            // commit date
    char* message;        // commit message
    int start_byte;      // start byte of commit in contents.txt
    int end_byte;       // end byte of commit in contents.txt
    FileHead* file;    // file head
    Commit* parent;  // parent commit
    Commit* child;  // child commit
};

struct commit_tree {
    Commit* root;      // root commit
};

struct file_head {
    File* first;   // head of file
    File* last;  // tail of file
};

struct file {
    char* path;   // path of file
    File* next;  // next file
};

// File functions
FileHead* file_create(void);
void file_insert(FileHead* head, const char* path);
void file_display(FileHead* head);
void file_destroy(FileHead* head);

// Commit functions
CommitTree* commit_tree_create(void);
void commit_tree_insert(CommitTree *tree, const char* hash, const char* date, const char* message, int start_byte, int end_byte, FileHead* file, Commit* parent);
void commit_tree_display(CommitTree* tree);
void commit_tree_destroy(CommitTree* tree);


void printInfo(const char* message);
void printAlert(const char* message);
void printWarning(const char* message);

#endif
