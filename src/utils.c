# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <time.h>

# include "utils.h"

// generates a random hash
char *get_hash() {
    char *hash = malloc(41 * sizeof(char));
    if (hash == NULL) {
        printf("Error allocating memory for hash\n");
        exit(EXIT_FAILURE);
    }

    const char *hexChars = "0123456789ABCDEF";
    srand(time(NULL));

    for (int i = 0; i < 40; i++) {
        hash[i] = hexChars[rand() % 16];
    }
    hash[40] = '\0';

    return hash;
}

// generate timestamp
char *get_timestamp() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}