# ifndef __FILES_H__
# define __FILES_H__

// Function prototypes
int create_directory(const char *path);
void write_file(const char *path);
FILE* open_file(const char* path, const char* mode);
void close_file(FILE* file);
int path_already_added(const char* file_path, const char* output_file);

# endif