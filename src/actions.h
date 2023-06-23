# ifndef _VCS_ACTIONS_H
# define _VCS_ACTIONS_H

# define INIT "init"
# define ADD "add"
# define COMMIT "commit"
# define LOG "log"
# define CHECKOUT "checkout"

void vcs_init();

void vcs_add(char *file);

void vcs_commit(char *message);

void vcs_log();

void vcs_checkout(char *commit_id);

# endif