# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# include "init.h"

int main(int argc, char const *argv[])
{
    init();
    return 0;
}

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

// verifies if folder exists
int folder_exists(char *path) {
    FILE *folder = fopen(path, "r");
    if (folder == NULL) {
        return 0;
    }
    fclose(folder);
    return 1;
}

// creates file
void create_file(char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("vcs: error: could not create file %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// on going commit
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

// generate commit
Commit *generate_commit(char *message) {
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    if (commit == NULL) {
        printf("vcs: error: could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    commit->hash = get_hash();
    commit->date = get_date();
    commit->message = message;
    commit->files = NULL;
    commit->next = NULL;
    commit->prev = NULL;
    return commit;
}

// appends commit metadata to .vcs/commits.txt file
void append_commit_metadata(Commit *commit) {
    FILE *commits = fopen(COMMITS_FILE, "a");
    if (commits == NULL) {
        printf("vcs: error: could not open .vcs/commits.txt\n");
        exit(EXIT_FAILURE);
    }
    fprintf(commits, "ID: %s\n", commit->hash);
    fprintf(commits, "Files: \n");
    fprintf(commits, "Date: %s\n", commit->date);
    fprintf(commits, "Message: %s\n", commit->message);
    fprintf(commits, "\n");
    fclose(commits);
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


// appends commit metadata to .vcs/logs.txt file
void append_commit_log(Commit *commit) {
    FILE *logs = fopen(LOGS_FILE, "a");
    if (logs == NULL) {
        printf("vcs: error: could not open .vcs/logs.txt\n");
        exit(EXIT_FAILURE);
    }
    fprintf(logs, "commit %s\n", commit->hash);
    fprintf(logs, "Date: %s\n", commit->date);
    fprintf(logs, "\n");
    fprintf(logs, "   %s\n", commit->message);
    fprintf(logs, "\n");
    fclose(logs);
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

// free commit



// initializes version control system repository
void init() {
    // verifies if .vcs folder already exists
    if (folder_exists(VCS_FOLDER) == 1){
        printf("vcs: error: .vcs folder already exists\n");
        exit(EXIT_FAILURE);
    }

    // creates .vcs folder using system
    system("mkdir .vcs");

    // creates .vcs/commits.txt file
    create_file(COMMITS_FILE);

    // creates .vcs/contents.txt file
    create_file(CONTENTS_FILE);

    // creates .vcs/logs.txt file
    create_file(LOGS_FILE);

    // generate initial commit
    Commit *commit = generate_commit("initial commit");

    // adds initial commit metadata to .vcs/commits.txt file
    append_commit_metadata(commit);

    // adds initial commit metadata to .vcs/logs.txt file
    append_commit_log(commit);

    // Generates on going commit
    commit->next = on_going_commit();

    // appends on going commit metadata to .vcs/commits.txt file
    append_on_going_commit_metadata(commit->next);

    // adds on going commit metadata to .vcs/logs.txt file
    append_on_going_commit_log(commit->next);

}

