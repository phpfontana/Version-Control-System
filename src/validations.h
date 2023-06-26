# ifndef __VALIDATIONS_H__
# define __VALIDATIONS_H__

/// Paths
# define VCS_DIRECTORY ".vcs"
# define COMMITS_FILE ".vcs/commits.txt"
# define STAGE_FILE ".vcs/stage.txt"
# define CONTENTS_FILE ".vcs/contents.txt"
# define METADATA_FILE ".vcs/metadata.txt"

/// FUNCTIONS
int directory_exists(const char *path);
int file_exists(const char *path);
int validate_directory(const char *path);
int file_is_empty(const char *path);
int path_is_staged(const char *path);

# endif