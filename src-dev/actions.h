# ifndef __VCS_ACTIONS_H__
# define __VCS_ACTIONS_H__

# define INIT "init"
# define ADD "add"
# define COMMIT "commit"
# define LOG "log"
# define CHECKOUT "checkout"
# define SHOW "show"

void vcs_init();

void vcs_add(const char* file_path);

void vcs_commit(const char *message);

void vcs_log();

void vcs_checkout(const char *hash);

void vcs_show(const char *id);

# endif