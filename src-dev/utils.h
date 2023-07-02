# ifndef __UTILS_H__
# define __UTILS_H__

# include <stdio.h>

/// Paths
# define VCS_DIRECTORY ".vcs"
# define COMMITS_FILE ".vcs/commits.txt"
# define STAGE_FILE ".vcs/stage.txt"
# define CONTENTS_DIRECTORY ".vcs/contents/"
# define METADATA_FILE ".vcs/metadata.txt"
# define EMPTY ""

/// FILE OPERATIONS
int create_directory(const char *path);
int write_file(const char *path, const char *content);
int append_file(const char *path, const char *content);
char *read_file(const char *path);
char *read_file_contents(const char *path, int start_byte, int end_byte);

/// VALIDATIONS
int directory_exists(const char *path);
int file_exists(const char *path);
int validate_directory(const char *path);
int file_is_empty(const char *path);
int path_is_staged(const char *path);

// COMMIT HELPER FUNCTIONS
char *hash();
char* concat_strings(const char* str1, const char* str2);
char* timestamp();

# endif