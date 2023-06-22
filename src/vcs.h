# ifndef _VERSION_CONTROL_SYSTEM_H_
# define _VERSION_CONTROL_SYSTEM_H_

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

# endif