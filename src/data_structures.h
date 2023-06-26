# ifndef __DATA_STRUCTURES_H__
# define __DATA_STRUCTURES_H__

typedef struct commit_head CommitHead;
typedef struct commit Commit;

typedef struct file_head FileHead;
typedef struct file File;

struct commit_head{
    Commit* first;
    Commit* last;
};

struct commit{
    char* hash;
    char* date;
    char* message;
    int start_byte;
    int end_byte;
    FileHead* file_head;
    Commit* next;
    Commit* prev;
};

struct file_head {
    File* first;
    File* last;
};
struct file {
    char* path;
    int start_byte;
    int end_byte;
    File* next;
    File* prev;
};

// function prototypes
CommitHead* commit_create();
void commit_insert(CommitHead* head, char* hash, char* date, const char* message, int start_byte, int end_byte);
void commit_display(CommitHead* head);
void commit_destroy(CommitHead* head);

FileHead* file_create();
void file_insert(FileHead* head, const char* path, int start_byte, int end_byte);
void file_display(FileHead* head);
void file_destroy(FileHead* head);

void display_commits(CommitHead* commit_head); 
void display_from_last(CommitHead* commit_head);
void display_from_last_with_contents(CommitHead* commit_head);
void display_commit_and_contents(CommitHead* commit_head, const char* hash);
void write_commit_and_contents(CommitHead* commit_head, const char* hash);

# endif