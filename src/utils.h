# ifndef _VCS_UTILS_H
# define _VCS_UTILS_H

// DATA STRUCTURES
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

// FILE PATHS
# define INIT 'init'
# define VCS_FOLDER ".vcs"
# define COMMITS_FILE ".vcs/commits.txt"
# define CONTENTS_FILE ".vcs/contents.txt"
# define LOGS_FILE ".vcs/logs.txt"
# define METADATA_FILE ".vcs/metadata.txt"
# define STAGE_FILE ".vcs/stage.txt"

// VCS INIT FUNCTIONS
int directory_exists(char *path);
int create_directory(char *path);
void create_file(char *path);

// VCS ADD FUNCTIONS
int directory_exists(char *path);
int file_exists(char *path);
int path_already_added(const char* file_path, const char* output_file);
void add_path_to_file(char* file_path, char* output_file);
    
// VCS COMMIT FUNCTIONS
int directory_exists(char *path);
int file_exists(char *path);
int validate_dir(void);
void create_file(char *path);
int is_empty(char *path) ;
char *get_hash();
Commit *init_commit(char* message);
void insert_file(Commit *commit, char *file_path);
void read_stage(Commit *commit);
void printFiles(struct files *head);
void append_contents(Files *files);
void append_commit(Commit *commit);
void append_metadata(Commit *commit);

// VCS LOG FUNCTIONS

# endif