# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include "commit.h"
# include "vcs.h"
# include "init.h"

int main(int argc, char const *argv[])
{
    commit("my second commit");
    return 0;
}

// verifies if folder exists
int folder_exists(char *path) {
    FILE *folder = fopen(path, "r");
    if (folder == NULL) {
        return 0;
    }
    fclose(folder);
    return 1;
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

// generates on going commit
Commit *on_going_commit(void) {
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    commit->hash = get_hash();
    commit->date = NULL;
    commit->message = NULL;
    commit->files = NULL;
    commit->next = NULL;
    commit->prev = NULL;
    return commit;
}

// append on going commit metadata to .vcs/commits.txt file
void append_on_going_commit_metadata (Commit *commit) {
    FILE *commits = fopen(COMMITS_FILE, "a");
    if (commits == NULL) {
        printf("vcs: error: could not open .vcs/commits.txt\n");
        exit(EXIT_FAILURE);
    }
    fprintf(commits, "ID: %s\n", commit->hash);    
    fprintf(commits, "Files: \n");
    fclose(commits);
}

// appends on going commit metadata to .vcs/logs.txt file
void append_on_going_commit_log(Commit *commit) {
    FILE *logs = fopen(LOGS_FILE, "a");
    if (logs == NULL) {
        printf("vcs: error: could not open .vcs/logs.txt\n");
        exit(EXIT_FAILURE);
    }
    fprintf(logs, "commit %s\n", commit->hash);
    fprintf(logs, "\n");
    fclose(logs);
}

// verifies on going commit

// get current date and time
char *get_date() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}

void commit(char *message) 
{
    // verifies if .vcs folder exists
    if (folder_exists(VCS_FOLDER) == 0) {
        printf("vcs: error: .vcs folder does not exist\n");
        exit(EXIT_FAILURE);
    }

    // init on going commit  
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    commit->hash = NULL;
    commit->date = get_date();
    commit->message = message;
    commit->files = NULL;
    commit->next = NULL;
    commit->prev = NULL;

    // save file contents to contents.txt

    // appends date and message to end of commits.txt
    FILE *commits = fopen(COMMITS_FILE, "a");
    if (commits == NULL) {
        printf("vcs: error: could not open .vcs/commits.txt\n");
        exit(EXIT_FAILURE);
    }
    fprintf(commits, "Date: %s\n", commit->date);
    fprintf(commits, "Message: %s\n", commit->message);
    fprintf(commits, "\n");
    fclose(commits);

    // appends date and message to end of logs.txt
    FILE *logs = fopen(LOGS_FILE, "a");
    if (logs == NULL) {
        printf("vcs: error: could not open .vcs/logs.txt\n");
        exit(EXIT_FAILURE);
    }

    fprintf(logs, "Date: %s\n", commit->date);
    fprintf(logs, "\n");
    fprintf(logs, "   %s\n", commit->message);
    fprintf(logs, "\n");
    fclose(logs);

    // Generates on going commit
    commit->next = on_going_commit();

    // appends on going commit metadata to .vcs/commits.txt file
    append_on_going_commit_metadata(commit->next);

    // adds on going commit metadata to .vcs/logs.txt file
    append_on_going_commit_log(commit->next);


}
