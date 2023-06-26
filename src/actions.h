# ifndef __ACTIONS_H__
# define __ACTIONS_H__

void vcs_init();
void vcs_add(const char *path);
void vcs_commit(const char *message);
void vcs_log(void);
void vcs_log_content(void);
void vcs_checkout(const char *hash);
void vcs_checkout_current(void);
void vcs_show(const char *hash);

# endif