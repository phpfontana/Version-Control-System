#ifndef VERSION_CONTROL_H
#define VERSION_CONTROL_H

#define MAX_FILES 10
#define MAX_FILENAME_LENGTH 20

// Global Variables
char repositoryPath[100] = "./repository";
char currentBranch[10] = "main";
char parentCommitID[10] = "";

// Structures
typedef struct Commit {
    char commitID[10];
    char parentCommitID[10];
    char branch[10];
} Commit;

typedef struct Branch {
    char name[10];
    Commit* latestCommit;
} Branch;

typedef struct TreeNode {
    Commit* commit;
    struct TreeNode* child;
    struct TreeNode* sibling;
} TreeNode;

// Function Declarations
void initializeRepository();
void loadRepositoryState();
void saveRepositoryState();
void commit();
void createBranch(const char* branchName);
void switchBranch(const char* branchName);
void removeBranch(const char* branchName);
void displayCommit(const char* commitID);
void displayBranch(const char* branchName);
void displayCommitHistory(TreeNode* node, int depth);
void cleanupCommitTree(TreeNode* node);

#endif /* VERSION_CONTROL_H */
