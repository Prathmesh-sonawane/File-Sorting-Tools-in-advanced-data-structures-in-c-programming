#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

// Define a structure to store file attributes.
struct FileInfo {
    char name[256];
    unsigned long long int size; // Change data type to unsigned long long int
    time_t date;
};


// Define a node for the binary search tree.
struct TreeNode {
    struct FileInfo data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Declare the inOrderTraversal function at the top of your code.
void inOrderTraversal(struct TreeNode* root);

// Function to insert a new file into the binary search tree based on size.
struct TreeNode* insertBySize(struct TreeNode* root, struct FileInfo data) {
    if (root == NULL) {
        struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        newNode->data = data;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (data.size < root->data.size)
        root->left = insertBySize(root->left, data);
    else
        root->right = insertBySize(root->right, data);

    return root;
}


// Function to insert a new file into the binary search tree based on date.
struct TreeNode* insertByDate(struct TreeNode* root, struct FileInfo data) {
    if (root == NULL) {
        struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        newNode->data = data;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (data.date < root->data.date)
        root->left = insertByDate(root->left, data);
    else
        root->right = insertByDate(root->right, data);

    return root;
}

// Function to insert a new file into the binary search tree based on name.
struct TreeNode* insertByName(struct TreeNode* root, struct FileInfo data) {
    if (root == NULL) {
        struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        newNode->data = data;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    int compareResult = strcmp(data.name, root->data.name);

    if (compareResult < 0)
        root->left = insertByName(root->left, data);
    else
        root->right = insertByName(root->right, data);

    return root;
}

// Function to read files in a directory and insert them into the binary search tree.
void readAndSortFiles(const char* dirPath, struct TreeNode* (*insertFunction)(struct TreeNode*, struct FileInfo)) {
    struct dirent* entry;
    DIR* dp = opendir(dirPath);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    struct TreeNode* root = NULL;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG) {
            struct FileInfo fileInfo;
            snprintf(fileInfo.name, sizeof(fileInfo.name), "%s/%s", dirPath, entry->d_name);

            struct stat fileStat;
            if (stat(fileInfo.name, &fileStat) == 0) {
                fileInfo.size = fileStat.st_size;
                fileInfo.date = fileStat.st_mtime;

                root = insertFunction(root, fileInfo);
            }
        }
    }

    closedir(dp);

    printf("\nSorted Files:\n");
    inOrderTraversal(root);

    // Free memory by deleting the binary search tree.
}

// Function to convert a timestamp to a human-readable date format.
char* formatTime(time_t timestamp) {
    char* formattedTime = ctime(&timestamp);
    if (formattedTime[strlen(formattedTime) - 1] == '\n') {
        formattedTime[strlen(formattedTime) - 1] = '\0'; // Remove the newline character
    }
    return formattedTime;
}

// In your inOrderTraversal function, use formatTime for displaying dates.
void inOrderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("Name: %s, Size: %llu, Date: %s\n\n", root->data.name, root->data.size, formatTime(root->data.date));
        inOrderTraversal(root->right);
    }
}

int main() {
    char dirPath[256];
    int sortCriteria;

    printf("Enter the directory path: ");
    scanf(" %[^\n]s", dirPath); // Read the path with spaces

    printf("Select sorting criteria:\n");
    printf("1 - Sort by name\n");
    printf("2 - Sort by size\n");
    printf("3 - Sort by date\n");
    printf("Enter your choice: ");
    scanf("%d", &sortCriteria);

    if (sortCriteria < 0 || sortCriteria > 2) {
        printf("Invalid choice. Exiting...\n");
        return 1;
    }

    if (sortCriteria == 1) {
        readAndSortFiles(dirPath, insertByName);
    } else if (sortCriteria == 2) {
        readAndSortFiles(dirPath, insertBySize);
    } else if (sortCriteria == 3) {
        readAndSortFiles(dirPath, insertByDate);
    } else {
        printf("Invalid choice. Exiting...\n");
    }

    return 0;
}
