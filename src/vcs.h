# ifndef _VERSION_CONTROL_SYSTEM_H_
# define _VERSION_CONTROL_SYSTEM_H_

# include <stdio.h>
# include <stdlib.h>
# include <string.h>


typedef struct commit Commit;
typedef struct files Files;

struct commit {
    char *hash;
    char *message;
    char *date;
    Files *files;
    Commit *next;
    Commit *prev;
};

struct files {
    char *path;
    FILE* file;
    Files *next;
    Files *prev;
};

# endif