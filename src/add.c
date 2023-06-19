# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include "add.h"
# include "vcs.h"
# include "init.h"

int main(int argc, char const *argv[])
{
    // add file to staging area
    add("vcs.h, add.c, add.h, commit.h");
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


// appends file or files to staging area
void add(char* file_path) {

    // verifies if .vcs folder exists
    if (folder_exists(VCS_FOLDER) == 0) {
        printf("vcs: error: .vcs folder does not exist\n");
        exit(EXIT_FAILURE);
    } 

    // append file paths to the end of .vcs/commits.txt
    FILE *commits = fopen(COMMITS_FILE, "a");
    if (commits == NULL) {
        printf("vcs: error: could not open .vcs/commits.txt\n");
        exit(1);
    }

    char *filePath;
    char* filePathsCopy = strdup(file_path);
    filePath = strtok(filePathsCopy, ", ");
    while (filePath != NULL) {
        // verifies if file exists
        FILE *file = fopen(filePath, "r");
        if (file == NULL) {
            printf("vcs: error: file %s does not exist\n", filePath);
            exit(1);
        }
        fclose(file);
        fprintf(commits, "%s\n", filePath); // Append the file path to the output file
        filePath = strtok(NULL, ", ");
    }
    fclose(commits);
    free(filePathsCopy);
}





