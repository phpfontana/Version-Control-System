#include <stdio.h>
#include <string.h>

#include "init.h"
#include "add.h"
#include "commit.h"
#include "log.h"
#include "checkout.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [<arguments>]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        vcs_init();
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf("Please provide file paths to add.\n");
            return 1;
        }
        vcs_add(argv[2]);
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc < 3) {
            printf("Please provide a commit message.\n");
            return 1;
        }
        vcs_commit(argv[2]);
    } else if (strcmp(argv[1], "log") == 0) {
        vcs_log();
    } else if (strcmp(argv[1], "checkout") == 0) {
        if (argc < 3) {
            printf("Please provide a commit ID.\n");
            return 1;
        }
        vcs_checkout(argv[2]);
    } else {
        printf("Invalid command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
