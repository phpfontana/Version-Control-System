# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <time.h>

# include "init.h"

int main(int argc, char const *argv[])
{
    vcs_init();
    return 0;
}


// verifies if directory exists
int directory_exists(char *path) {
    int status = access(path, 0);
    if (status == 0)
        return 1;  // directory exists
    else
        return 0;  // directory does not exist
}

// creates a directory
int create_directory(char *path) {
    int status = mkdir(path, 0777);
    
    if (status == 0)
        return 1;  // directory created successfully
    else
        return 0;  // unable to create directory
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


// initializes version control system repository
void vcs_init() {

    // verifies if .vcs directory exists
    if (directory_exists(".vcs") == 1) {
        printf("vcs: error: .vcs directory already exists\n");
        exit(EXIT_FAILURE);
    }
    
    // creates .vcs directory
    if (create_directory(".vcs") == 0) {
        printf("vcs: error: could not create .vcs directory\n");
        exit(EXIT_FAILURE);
    }
    
    // creates .vcs/commits.txt file
    create_file(COMMITS_FILE);

    // creates .vcs/stage.txt file
    create_file(STAGE_FILE);

    // creates .vcs/logs.txt file
    create_file(LOGS_FILE);

    // creates .vcs/content.txt file
    create_file(CONTENTS_FILE);

    // creates .vcs/metadata.txt file
    create_file(METADATA_FILE);

    // prints success message
    printf("vcs: initialized empty version control system repository in .vcs\n");
}



