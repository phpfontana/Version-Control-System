/**
 * @file utils.c
 * @brief Utility functions for the VCS (Version Control System).
 *
 * This file contains utility functions used in the VCS system.
 */
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>
# include <sys/stat.h>

# include "utils.h"

/**
 * @brief Generates a random hexadecimal hash.
 *
 * @return A dynamically allocated string containing the generated hash.
 */
char *hash() {
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

/**
 * @brief Retrieves the current timestamp.
 *
 * @return A string representing the current timestamp.
 */
char* timestamp() {
    time_t t;
    time(&t);
    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';
    return date;
}

/**
 * @brief Prints an informational message.
 *
 * @param message The message to be printed.
 */
void printInfo(const char* message) {
    printf("[INFO] %s\n", message);
}

/**
 * @brief Prints an alert message.
 *
 * @param message The message to be printed.
 */
void printAlert(const char* message) {
    printf("[ALERT] %s\n", message);
}

/**
 * @brief Prints a warning message.
 *
 * @param message The message to be printed.
 */
void printWarning(const char* message) {
    printf("[WARNING] %s\n", message);
}
