# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "add.h"

// adds file or files to staging area
void add(char* path)
{
    // verifies if .vcs directory, or .vcs/commits.txt file or .vcs/contents directory already exists
    if (system("test -d .vcs") != 0 || system("test -f .vcs/commits.txt") != 0 || system("test -d .vcs/contents") != 0) {
        printf("vcs: error: repository not initialized\n");
        exit(1);
    }
    
    // verifies if file exists
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("vcs: error: file not found\n");
        exit(1);
    }
    fclose(file);

    // verifies if file is already in staging area commits.txt
    file = fopen(".vcs/commits.txt", "r");
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, path) == 0) {
            printf("vcs: error: file already in staging area\n");
            exit(1);
        }
    }
    fclose(file);

    // adds file to staging area
    file = fopen(".vcs/commits.txt", "a");
    fprintf(file, "%s\n", path);
    fclose(file);
} 

int main(int argc, char const *argv[])
{
    // add file to staging area
    add("test.txt");
    return 0;
}


