# ifndef __PARSE_H__
# define __PARSE_H__

# include "data_structures.h"

// Function prototypes
void parse_file_paths_to_stage(const char* file_path);
void parse_stage_to_files(Commit* commit);
void parse_files_to_contents (Files* file);
void parse_commit_to_commit_file(Commit* commit);
void parse_commit_to_metadata_file(Commit* commit);
void parse_commit_file_to_commit(Commit** head);
void parse_metadata_file_to_commit(const char *hash, int byte_start, int byte_end);
void parse_commit_to_files(int byte_start, int byte_end);

Files *parseFiles(FILE *commitsFile, int endByte);
void freeFiles(Files *headFile);
void extractContentFromFile(const char* file_path, int start_byte, int end_byte, const char* output_file);

Files *parseFiles_(FILE *commitsFile, int endByte);
void extractContentFromFile_print(const char* file_path, int start_byte, int end_byte);
void print_commits_(Commit* head);

# endif