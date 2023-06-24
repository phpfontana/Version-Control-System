# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "utils.h"
# include "actions.h"


int main(int argc, char const *argv[])
{
    char const *action = argv[1];

    if (strcmp(action, INIT) == 0) {
        vcs_init();
    } else if (strcmp(action, ADD) == 0) {
        vcs_add(argv[2]);
    } else if (strcmp(action, COMMIT) == 0) {
        vcs_commit(argv[3]);
    } else if (strcmp(action, LOG) == 0) {
        vcs_log();
    } else if (strcmp(action, CHECKOUT) == 0) {
        vcs_checkout(argv[2]);
    } else if (strcmp(action, SHOW) == 0) {
        vcs_show(argv[2]);
    } else {
        printf("vcs: error: invalid action\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}


