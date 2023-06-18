# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# include "init.h"

// get current date and time
char *get_date() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}

// generates a random hash
char *get_hash() {
    char *hash = malloc(41 * sizeof(char));
    for (int i = 0; i < 40; i++) {
        hash[i] = (rand() % 16) + '0';
    }
    hash[40] = '\0';
    return hash;
}

// initializes version control system repository
void init() {
    // verifies if .vcs directory, or .vcs/commits.txt file or .vcs/contents or .vcs/logs.txt file already exists
    if (system("test -d .vcs") == 0 || system("test -f .vcs/commits.txt") == 0 || system("test -d .vcs/contents") == 0 || system("test -f .vcs/logs.txt") == 0) {
        printf("vcs: error: repository already initialized\n");
        exit(1);
    }
    // create .vcs directory
    system("mkdir .vcs");
    // create .vcs/commits.txt file
    system("touch .vcs/commits.txt");
    // create .vcs/contents directory
    system("mkdir .vcs/contents"); 
    // create .vcs/logs.txt file
    system("touch .vcs/logs.txt");

    // generates initial commit metadata
    char *hash = get_hash();
    char *date = get_date();
    char *message = "initial commit";

    // adds initial commit metadata to .vcs/commits.txt file and save it to commit struct
    FILE *commits = fopen(".vcs/commits.txt", "w");
    if (commits == NULL) {
        printf("vcs: error: could not open .vcs/commits.txt\n");
        exit(1);
    }
    fprintf(commits, "ID: %s\n", hash);
    fprintf(commits, "Date: %s\n", date);
    fprintf(commits, "Message: %s\n", message);
    fprintf(commits, "Files: \n");
    fprintf(commits, "\n");
    fclose(commits);

    // adds initial commit metadata to .vcs/logs.txt file
    FILE *logs = fopen(".vcs/logs.txt", "w");
    if (logs == NULL) {
        printf("vcs: error: could not open .vcs/logs.txt\n");
        exit(1);
    }
    fprintf(logs, "commit %s\n", hash);
    fprintf(logs, "Date: %s\n", date);
    fprintf(logs, "\n");
    fprintf(logs, "   %s\n", message);
    fclose(logs);

    // free memory
    free(hash);
    free(date);

    printf("vcs: repository initialized\n");

    exit(0);
}

int main(int argc, char const *argv[])
{
    init();
    return 0;
}

