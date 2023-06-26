# ifndef __PARSER_H__
# define __PARSER_H__

# include "data_structures.h"

// Function prototypes
void parse_stage(FileHead *head);
void parse_commit_file(CommitHead *commit_head, FileHead *file_head, const char *message);
void parse_commits(CommitHead *commit_head);
void print_text_between_bytes(int byte_start, int byte_end);
void write_text_between_bytes(int byte_start, int byte_end, const char *path);
# endif