# ifndef __FILES_H__
# define __FILES_H__

# include <stdio.h>

// FUNCTIONS
int create_directory(const char *path);
int write_empty_file(const char *path);
int write_file(const char *path, const char *content);
FILE *open_file(const char *path, const char *mode);
int append_to_file(const char *path, const char *content);
char *read_file(const char *path);
# endif
