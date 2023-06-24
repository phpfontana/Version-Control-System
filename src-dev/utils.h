# ifndef __VCS_UTILS_H__
# define __VCS_UTILS_H__

/// DATA STRUCTURES
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

// CONSTANTS
#define VCS_DIRECTORY ".vcs"
#define COMMITS_FILE ".vcs/commits.txt"
#define STAGE_FILE ".vcs/stage.txt"
#define CONTENTS_FILE ".vcs/content.txt"
#define METADATA_FILE ".vcs/metadata.txt"

// VCS INIT FUNCTIONS
int directory_exists(const char *path);
int create_directory(const char *path);
void write_file(const char *path);

// VCS ADD FUNCTIONS
int file_exists(const char* path);
int path_already_added(const char* file_path, const char* output_file);
void add_path_to_file(const char* file_path, char* output_file);

/// VCS COMMIT FUNCTIONS
int validate_directory(void);
int is_empty(char *path);
char *get_hash();
char *get_timestamp();
Commit *init_commit(const char* message);
void insert_file(Commit *commit, char *file_path);
void read_stage(Commit *commit);
void printFiles(struct files *head);
void append_contents(Files *files);
void append_commit(Commit *commit);
void append_metadata(Commit *commit);

// VCS LOG FUNCTIONS
void read_commits(const char* commits_file, Commit** head);
void print_commits(Commit* head);
void free_commits(Commit* head);

// VCS CHECKOUT
Files *parseFiles(FILE *commitsFile, int endByte);
void freeFiles(Files *headFile);
void extractContentFromFile(const char* file_path, int start_byte, int end_byte, const char* output_file);

/// VCS SHOW
Files* parseFiles_(FILE* commitsFile, int endByte);
void extractContentFromFile_print(const char* file_path, int start_byte, int end_byte);
void print_commits_(Commit* head);
void vcs_show(const char* id);

# endif