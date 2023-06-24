# ifndef __DATA_STRUCTURES_H__
# define __DATA_STRUCTURES_H__

typedef struct commit Commit;
typedef struct files Files;

struct commit {
    char *hash;
    char *date;
    Files *files;  // head file
    char *message;
    int byte_start; // start byte on commits.txt
    int byte_end; // end byte on commits.txt
    Commit *next;  // next commit
    Commit *prev;  // previous commit
};

struct files {
    char *file_path;
    int byte_start;
    int byte_end;
    Files *next;
    Files *prev;
};

// Function prototypes
Commit *new_commit(const char* hash, const char* message, const char* date);
Files *new_file(const char *file_path);
void insert_file (Commit* commit, Files* file);

# endif