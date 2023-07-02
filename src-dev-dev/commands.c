# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>
# include "commands.h"

// Function Implementations
void initializeRepository() {
    if (mkdir(repositoryPath, 0700) != 0) {
        fprintf(stderr, "Failed to create repository directory.\n");
        exit(1);
    }
    if (mkdir("./repository/commits", 0700) != 0) {
        fprintf(stderr, "Failed to create commits directory.\n");
        exit(1);
    }
    if (mkdir("./repository/branches", 0700) != 0) {
        fprintf(stderr, "Failed to create branches directory.\n");
        exit(1);
    }
    loadRepositoryState();
}

void loadRepositoryState() {
    char configFilePath[100];
    sprintf(configFilePath, "%s/config.txt", repositoryPath);
    FILE* configFile = fopen(configFilePath, "r");
    if (configFile != NULL) {
        fscanf(configFile, "Current Branch: %s\n", currentBranch);
        fscanf(configFile, "Parent Commit ID: %s\n", parentCommitID);
        fclose(configFile);
    }
}

void saveRepositoryState() {
    char configFilePath[100];
    sprintf(configFilePath, "%s/config.txt", repositoryPath);
    FILE* configFile = fopen(configFilePath, "w");
    if (configFile == NULL) {
        fprintf(stderr, "Failed to save repository state.\n");
        exit(1);
    }
    fprintf(configFile, "Current Branch: %s\n", currentBranch);
    fprintf(configFile, "Parent Commit ID: %s\n", parentCommitID);
    fclose(configFile);
}

void commit() {
    Commit* newCommit = (Commit*)malloc(sizeof(Commit));
    strcpy(newCommit->branch, currentBranch);
    strcpy(newCommit->parentCommitID, parentCommitID);
    sprintf(newCommit->commitID, "C%d", rand() % 1000);  // Random commit ID

    // Save commit data to file
    char commitFilePath[100];
    sprintf(commitFilePath, "%s/commits/%s.txt", repositoryPath, newCommit->commitID);
    FILE* commitFile = fopen(commitFilePath, "w");
    if (commitFile == NULL) {
        fprintf(stderr, "Failed to create commit file.\n");
        exit(1);
    }
    fprintf(commitFile, "Commit ID: %s\n", newCommit->commitID);
    fprintf(commitFile, "Branch: %s\n", newCommit->branch);
    fprintf(commitFile, "Parent Commit ID: %s\n", newCommit->parentCommitID);
    fclose(commitFile);

    // Update the latest commit for the current branch
    Branch currentBranchStruct;
    strcpy(currentBranchStruct.name, currentBranch);
    currentBranchStruct.latestCommit = newCommit;

    // Save the updated branch data to file
    char branchFilePath[100];
    sprintf(branchFilePath, "%s/branches/%s.txt", repositoryPath, currentBranch);
    FILE* branchFile = fopen(branchFilePath, "w");
    if (branchFile == NULL) {
        fprintf(stderr, "Failed to update branch file.\n");
        exit(1);
    }
    fprintf(branchFile, "%s", newCommit->commitID);
    fclose(branchFile);

    // Update parent commit for the next commit
    strcpy(parentCommitID, newCommit->commitID);
    saveRepositoryState();

    printf("New commit created with ID: %s\n", newCommit->commitID);
}

void createBranch(const char* branchName) {
    char branchFilePath[100];
    sprintf(branchFilePath, "%s/branches/%s.txt", repositoryPath, branchName);

    FILE* branchFile = fopen(branchFilePath, "r");
    if (branchFile != NULL) {
        fclose(branchFile);
        fprintf(stderr, "Branch with name %s already exists.\n", branchName);
        return;
    }

    branchFile = fopen(branchFilePath, "w");
    if (branchFile == NULL) {
        fprintf(stderr, "Failed to create branch file.\n");
        exit(1);
    }
    fclose(branchFile);

    printf("New branch created with name: %s\n", branchName);
}

void switchBranch(const char* branchName) {
    if (strcmp(currentBranch, branchName) == 0) {
        printf("Already on branch: %s\n", branchName);
        return;
    }

    char branchFilePath[100];
    sprintf(branchFilePath, "%s/branches/%s.txt", repositoryPath, branchName);

    FILE* branchFile = fopen(branchFilePath, "r");
    if (branchFile == NULL) {
        fprintf(stderr, "Branch with name %s does not exist.\n", branchName);
        return;
    }
    fclose(branchFile);

    // Update current branch
    strcpy(currentBranch, branchName);
    saveRepositoryState();

    printf("Switched to branch: %s\n", branchName);
}

void removeBranch(const char* branchName) {
    if (strcmp(currentBranch, branchName) == 0) {
        printf("Cannot remove the current branch.\n");
        return;
    }

    char branchFilePath[100];
    sprintf(branchFilePath, "%s/branches/%s.txt", repositoryPath, branchName);

    if (remove(branchFilePath) == 0) {
        printf("Branch %s has been removed.\n", branchName);
    } else {
        fprintf(stderr, "Failed to remove branch %s.\n", branchName);
    }
}

void displayCommit(const char* commitID) {
    char commitFilePath[100];
    sprintf(commitFilePath, "%s/commits/%s.txt", repositoryPath, commitID);
    FILE* commitFile = fopen(commitFilePath, "r");
    if (commitFile == NULL) {
        fprintf(stderr, "Commit with ID %s does not exist.\n", commitID);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), commitFile)) {
        printf("%s", line);
    }
    fclose(commitFile);
}

void displayCommitHistory(TreeNode* node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("Commit ID: %s\n", node->commit->commitID);

    displayCommitHistory(node->child, depth + 1);
    displayCommitHistory(node->sibling, depth);
}

void displayBranch(const char* branchName) {
    char branchFilePath[100];
    sprintf(branchFilePath, "%s/branches/%s.txt", repositoryPath, branchName);

    FILE* branchFile = fopen(branchFilePath, "r");
    if (branchFile == NULL) {
        fprintf(stderr, "Branch with name %s does not exist.\n", branchName);
        return;
    }

    char commitID[10];
    fscanf(branchFile, "%s", commitID);
    fclose(branchFile);

    if (strcmp(commitID, "") == 0) {
        printf("Branch: %s (No commits)\n", branchName);
        return;
    }

    TreeNode* commitTree = (TreeNode*)malloc(sizeof(TreeNode));
    commitTree->commit = NULL;
    commitTree->child = NULL;
    commitTree->sibling = NULL;

    TreeNode* current = commitTree;

    // Traverse commit history and build a tree structure
    while (strcmp(commitID, "") != 0) {
        Commit* commit = (Commit*)malloc(sizeof(Commit));
        strcpy(commit->commitID, commitID);
        strcpy(commit->branch, branchName);
        strcpy(commit->parentCommitID, "");

        // Load commit data from file
        char commitFilePath[100];
        sprintf(commitFilePath, "%s/commits/%s.txt", repositoryPath, commitID);
        FILE* commitFile = fopen(commitFilePath, "r");
        if (commitFile == NULL) {
            fprintf(stderr, "Commit with ID %s does not exist.\n", commitID);
            return;
        }
        fscanf(commitFile, "Commit ID: %s\n", commit->commitID);
        fscanf(commitFile, "Branch: %s\n", commit->branch);
        fscanf(commitFile, "Parent Commit ID: %s\n", commit->parentCommitID);
        fclose(commitFile);

        // Create a new node for the commit and link it in the tree structure
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->commit = commit;
        newNode->child = NULL;
        newNode->sibling = NULL;

        if (current->child == NULL) {
            current->child = newNode;
        } else {
            current = current->child;
            while (current->sibling != NULL) {
                current = current->sibling;
            }
            current->sibling = newNode;
        }

        // Update commit ID for the next iteration
        strcpy(commitID, commit->parentCommitID);
    }

    // Display the commit history in a depth-first manner
    printf("Branch: %s\n", branchName);
    printf("Commit History:\n");
    displayCommitHistory(commitTree, 0);

    // Clean up the commit tree
    cleanupCommitTree(commitTree);
}

void cleanupCommitTree(TreeNode* node) {
    if (node == NULL) {
        return;
    }

    cleanupCommitTree(node->child);
    cleanupCommitTree(node->sibling);
    free(node->commit);
    free(node);
}

void renameBranch(const char* oldBranchName, const char* newBranchName) {
    if (strcmp(currentBranch, oldBranchName) == 0) {
        printf("Cannot rename the current branch.\n");
        return;
    }

    char oldBranchFilePath[100];
    char newBranchFilePath[100];
    sprintf(oldBranchFilePath, "%s/branches/%s.txt", repositoryPath, oldBranchName);
    sprintf(newBranchFilePath, "%s/branches/%s.txt", repositoryPath, newBranchName);

    if (rename(oldBranchFilePath, newBranchFilePath) == 0) {
        printf("Branch %s has been renamed to %s.\n", oldBranchName, newBranchName);
    } else {
        fprintf(stderr, "Failed to rename branch %s.\n", oldBranchName);
    }
}

int main() {
    initializeRepository();

    commit();
    createBranch("feature1");
    switchBranch("feature1");
    commit();
    commit();
    createBranch("feature2");
    switchBranch("feature2");
    commit();
    switchBranch("feature1");
    commit();
    switchBranch("main");
    commit();

    displayBranch("main");
    printf("----------------------------\n");
    displayBranch("feature1");
    printf("----------------------------\n");
    displayBranch("feature2");

    removeBranch("feature1");
    renameBranch("feature2", "feature3");

    displayBranch("main");
    printf("----------------------------\n");
    displayBranch("feature1");
    printf("----------------------------\n");
    displayBranch("feature2");
    printf("----------------------------\n");
    displayBranch("feature3");


    return 0;
}
