# ifndef __ACTIONS_H__
# define __ACTIONS_H__

void vcs_init();

void vcs_add(const char* file_path);

void vcs_commit(const char *message);

void vcs_log();

void vcs_checkout(const char *hash);

void vcs_show(const char *id);

# endif